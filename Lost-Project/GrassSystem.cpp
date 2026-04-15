#include "GrassSystem.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <cstdio>
#include <vector>
#include <stdexcept>
#include "Random.h"
using namespace Engine;

// ============================================================
//  Inline GLSL shaders
// ============================================================

static const char* k_GrassVert = R"GLSL(
#version 330 core

// ---- Per-vertex (blade template, local space) ----
layout(location = 0) in vec3  aPos;        // local position
layout(location = 1) in float aV;          // 0=base .. 1=tip  (used for coloring & wind)

// ---- Per-instance ----
layout(location = 2) in vec2  iWorldXZ;    // world-space XZ origin of this blade
layout(location = 3) in float iHeightMul;  // noise-based height scale  (e.g. 0.6 – 1.4)
layout(location = 4) in float iRandAngle;  // random Y rotation (radians)

// ---- Uniforms ----
uniform mat4  uView;
uniform mat4  uProjection;
uniform float uTime;
uniform float uWindSpeed;
uniform float uWindStrength;
uniform float uWindFrequency;

// ---- Out to frag ----
out float vV;   // 0=base, 1=tip (for color lerp)
out float heightMul; 

void main()
{
    // Scale blade height by per-instance noise value
    vec3 pos = aPos;
    pos.y *= iHeightMul + 0.3;

    // Random Y rotation so blades don't all face the same way
    float c = cos(iRandAngle);
    float s = sin(iRandAngle);
    pos = vec3(c * pos.x - s * pos.z,
               pos.y,
               s * pos.x + c * pos.z);

    // Wind: sinusoidal displacement in X/Z, stronger at tip
    float windPhase = uTime * uWindSpeed
                    + iWorldXZ.x * uWindFrequency
                    + iWorldXZ.y * uWindFrequency * 0.7;
    float windBend = sin(windPhase) * uWindStrength * aV * aV; // quadratic falloff to base
    pos.x += windBend;
    pos.z += windBend * 0.4;

    // Place blade in world space
    vec3 worldPos = pos + vec3(iWorldXZ.x, 0.0, iWorldXZ.y);

    gl_Position = uProjection * uView * vec4(worldPos, 1.0);
    vV = aV;
    heightMul = iHeightMul;
}
)GLSL";

static const char* k_GrassFrag = R"GLSL(
#version 330 core

in  float vV;
in float heightMul;
out vec4  FragColor;

uniform vec3 uColorBase;
uniform vec3 uColorTip;

void main()
{
    if(heightMul < 0.01) discard; // alpha cutout for sharper edges (optional)
    // Simple vertical color gradient, base → tip
    vec3 color = mix(uColorBase, uColorTip, vV);

    // Fake ambient occlusion: darken base slightly
    color *= (0.55 + 0.45 * vV);

    FragColor = vec4(color, 1.0);
}
)GLSL";

// ============================================================
//  Noise utilities (CPU-side, all baked into instance buffer)
// ============================================================

// Integer hash → reproducible pseudo-random float in [-1, 1]
static float hash(int x, int z)
{
    int n = x * 1619 + z * 31337;
    n = (n << 13) ^ n;
    return 1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff)
                 / 1073741824.0f;
}

// Gradient vector for Perlin noise — 8 directions on the XZ plane
static glm::vec2 gradVec(int x, int z)
{
    // Map hash to one of 8 unit gradient directions
    int h = (int)((hash(x, z) * 0.5f + 0.5f) * 255.99f) & 7;
    const float dirs[8][2] = {
        { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1},
        { 0.7071f, 0.7071f}, {-0.7071f, 0.7071f},
        { 0.7071f,-0.7071f}, {-0.7071f,-0.7071f}
    };
    return glm::vec2(dirs[h][0], dirs[h][1]);
}

// Quintic fade: 6t^5 - 15t^4 + 10t^3  (smoother than cubic, no 2nd-derivative discontinuity)
static float fade(float t)
{
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

// Classic 2D Perlin gradient noise, returns [-1, 1]
float GrassSystem::perlinNoise(float x, float z)
{
    int xi = (int)std::floor(x);
    int zi = (int)std::floor(z);
    float tx = x - xi;
    float tz = z - zi;

    float ux = fade(tx);
    float uz = fade(tz);

    // Dot products of gradient vectors with distance vectors
    auto dot2 = [](glm::vec2 g, float dx, float dz) {
        return g.x * dx + g.y * dz;
    };

    float v00 = dot2(gradVec(xi,   zi),   tx,       tz      );
    float v10 = dot2(gradVec(xi+1, zi),   tx-1.0f,  tz      );
    float v01 = dot2(gradVec(xi,   zi+1), tx,       tz-1.0f );
    float v11 = dot2(gradVec(xi+1, zi+1), tx-1.0f,  tz-1.0f );

    // Bilinear interpolation
    float x0 = v00 + ux * (v10 - v00);
    float x1 = v01 + ux * (v11 - v01);
    return (x0 + uz * (x1 - x0)) * 1.4142f; // scale to roughly fill [-1,1]
}

// Fractional Brownian Motion — layered octaves of Perlin for richer detail
// Returns [0, 1]
float GrassSystem::fbm(float x, float z, int octaves, float lacunarity, float gain)
{
    float value    = 0.0f;
    float amplitude = 0.5f;
    float frequency = 1.0f;
    float maxVal    = 0.0f;

    for (int i = 0; i < octaves; ++i)
    {
        value   += perlinNoise(x * frequency, z * frequency) * amplitude;
        maxVal  += amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }
    // Normalize to [0, 1]
    return (value / maxVal) * 0.5f + 0.5f;
}

// Legacy value noise kept for the per-blade height jitter (high-frequency, cheap)
float GrassSystem::valueNoise(float x, float z)
{
    int xi = (int)std::floor(x);
    int zi = (int)std::floor(z);
    float tx = x - xi;
    float tz = z - zi;
    float ux = tx * tx * (3 - 2 * tx);
    float uz = tz * tz * (3 - 2 * tz);

    float v00 = hash(xi,   zi);
    float v10 = hash(xi+1, zi);
    float v01 = hash(xi,   zi+1);
    float v11 = hash(xi+1, zi+1);

    return  v00 * (1-ux) * (1-uz)
          + v10 *  ux    * (1-uz)
          + v01 * (1-ux) *  uz
          + v11 *  ux    *  uz;
}

// ============================================================
//  Blade mesh (shared template, instanced in world-space)
// ============================================================
//  Vertex layout: [ x, y, z,  v ]   (4 floats)
//  where v ∈ [0,1] is the normalised vertical position.
//
//  The blade is centred on X=0, Z=0, grows upward from Y=0.
//  Horizontal vertices form pairs (left/right) narrowing to a tip.
void GrassSystem::buildBladeMesh(std::vector<float>& verts,
                                  std::vector<unsigned int>& indices)
{
    verts.clear();
    indices.clear();

    int   segs   = std::max(config.segments, 2);
    float hw     = config.bladeWidth * 0.5f;
    float height = config.bladeHeight;

    // Generate vertices: segs+1 rows, each row = left + right vertex
    // Tip row = single vertex at centre.
    for (int row = 0; row <= segs; ++row)
    {
        float t = (float)row / segs;          // 0 = base, 1 = tip
        float y = t * height;
        float w = hw * (1.0f - t * 0.9f);    // blade narrows to 10% width at top

        if (row < segs)
        {
            // Left
            verts.push_back(-w);  verts.push_back(y); verts.push_back(0.0f); verts.push_back(t);
            // Right
            verts.push_back( w);  verts.push_back(y); verts.push_back(0.0f); verts.push_back(t);
        }
        else
        {
            // Tip – single centre vertex
            verts.push_back(0.0f); verts.push_back(y); verts.push_back(0.0f); verts.push_back(1.0f);
        }
    }

    // Build triangle list from the rows
    // Row i has vertices at indices: i*2, i*2+1
    // Tip vertex index = segs * 2
    int tipIdx = segs * 2;

    for (int row = 0; row < segs - 1; ++row)
    {
        int bl = row * 2;
        int br = row * 2 + 1;
        int tl = row * 2 + 2;
        int tr = row * 2 + 3;
        // Two triangles per quad
        indices.push_back(bl); indices.push_back(br); indices.push_back(tl);
        indices.push_back(br); indices.push_back(tr); indices.push_back(tl);
    }
    // Cap triangle to tip
    int lastLeft  = (segs - 1) * 2;
    int lastRight = (segs - 1) * 2 + 1;
    indices.push_back(lastLeft);
    indices.push_back(lastRight);
    indices.push_back(tipIdx);

    m_indexCount = (int)indices.size();
}

// ============================================================
//  Instance data  [ worldX, worldZ, heightMul, randAngle ]
// ============================================================
void GrassSystem::buildInstanceData(std::vector<float>& data)
{
    data.clear();
    data.reserve(config.countX * config.countZ * 4);

    for (int iz = 0; iz < config.countZ; ++iz)
    {
        for (int ix = 0; ix < config.countX; ++ix)
        {
            float wx = config.originX + ix * config.spacingX;
            float wz = config.originZ + iz * config.spacingZ;

            wx = Random::range(-50.0f, 50.0f);
            wz = Random::range(-50.0f, 50.0f);


            // ----------------------------------------------------------
            // 1. Density mask — large-scale fBm Perlin defines where
            //    grass grows.  Result is [0,1].
            //    Low frequency = broad meadow/clearing shapes.
            //    Higher octaves add raggedy patch edges.
            // ----------------------------------------------------------
            float density = fbm(
                wx * config.densityNoiseScale,
                wz * config.densityNoiseScale,
                config.densityOctaves,
                2.0f,   // lacunarity: each octave doubles frequency
                0.5f    // gain: each octave halves amplitude
            );

            // Remap: values below threshold → 0 (no grass), above → smooth ramp up to 1.
            // smoothstep gives soft patch edges instead of a hard cutoff.
            float t = (density - config.densityThreshold)
                    / (1.0f - config.densityThreshold);
            t = std::max(0.0f, std::min(1.0f, t));
            float densityMask = t * t * (3.0f - 2.0f * t); // smoothstep

            // ----------------------------------------------------------
            // 2. Per-blade height jitter — high-frequency value noise
            //    gives each blade a slightly different height within
            //    the patch.
            // ----------------------------------------------------------
            float n  = valueNoise(wx * config.noiseScale,
                                  wz * config.noiseScale);
            float n2 = valueNoise(wx * config.noiseScale * 2.3f + 7.1f,
                                  wz * config.noiseScale * 2.3f + 3.5f);
            float combined  = n * 0.7f + n2 * 0.3f;           // [-1, 1]
            float heightVar = 1.0f + combined * config.noiseAmp;

            // heightVar = 

            // ----------------------------------------------------------
            // 3. Combine: mask collapses height to 0 outside patches.
            //    The vertex shader scales pos.y by iHeightMul, so 0
            //    collapses the blade to a degenerate point at ground
            //    level — invisible, no discard needed.
            // ----------------------------------------------------------
            float heightMul = combined * densityMask;

            // Random Y rotation
            float rAngle = (hash(ix * 7 + 3, iz * 13 + 5) * 0.5f + 0.5f)
                         * 6.2831853f;

            float np = (hash(ix * 7 + 3, iz * 13 + 5) * 0.5f + 0.5f)
                         * 6.2831853f;

            data.push_back(wx + np * 0.01);
            data.push_back(wz + np * 0.01);
            data.push_back(heightMul);
            data.push_back(rAngle);
        }
    }

    m_instanceCount = config.countX * config.countZ;
}

// ============================================================
//  Shader helpers
// ============================================================
GLuint GrassSystem::compileShader(GLenum type, const char* src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);

    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char log[512];
        glGetShaderInfoLog(s, 512, nullptr, log);
        printf("[GrassSystem] Shader compile error:\n%s\n", log);
    }
    return s;
}

GLuint GrassSystem::linkProgram(GLuint vert, GLuint frag)
{
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    GLint ok = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char log[512];
        glGetProgramInfoLog(prog, 512, nullptr, log);
        printf("[GrassSystem] Program link error:\n%s\n", log);
    }
    glDeleteShader(vert);
    glDeleteShader(frag);
    return prog;
}

// ============================================================
//  Init
// ============================================================
void GrassSystem::Init()
{
    Init(GrassConfig{});
}

void GrassSystem::Init(const GrassConfig& cfg)
{
    config = cfg;

    // ----- Build CPU-side data -----
    std::vector<float>        bladeVerts;
    std::vector<unsigned int> bladeIdx;
    std::vector<float>        instanceData;

    buildBladeMesh(bladeVerts, bladeIdx);
    buildInstanceData(instanceData);

    // ----- GPU buffers -----
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_bladeVBO);
    glGenBuffers(1, &m_instanceVBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    // Blade vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_bladeVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 bladeVerts.size() * sizeof(float),
                 bladeVerts.data(),
                 GL_STATIC_DRAW);

    // layout 0: aPos (vec3)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // layout 1: aV (float)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void*)(3 * sizeof(float)));

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 bladeIdx.size() * sizeof(unsigned int),
                 bladeIdx.data(),
                 GL_STATIC_DRAW);

    // Per-instance buffer  [ worldX, worldZ, heightMul, randAngle ]
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 instanceData.size() * sizeof(float),
                 instanceData.data(),
                 GL_STATIC_DRAW);

    // layout 2: iWorldXZ (vec2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribDivisor(2, 1);

    // layout 3: iHeightMul (float)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void*)(2 * sizeof(float)));
    glVertexAttribDivisor(3, 1);

    // layout 4: iRandAngle (float)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);

    // ----- Shader -----
    GLuint vert = compileShader(GL_VERTEX_SHADER,   k_GrassVert);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, k_GrassFrag);
    m_shader = linkProgram(vert, frag);

    m_ready = true;
}

// ============================================================
//  Render
// ============================================================
void GrassSystem::Render(const glm::mat4& view,
                          const glm::mat4& proj,
                          float time)
{
    if (!m_ready) return;

    glUseProgram(m_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_shader, "uView"),       1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "uProjection"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniform1f(glGetUniformLocation(m_shader, "uTime"),          time);
    glUniform1f(glGetUniformLocation(m_shader, "uWindSpeed"),     config.windSpeed);
    glUniform1f(glGetUniformLocation(m_shader, "uWindStrength"),  config.windStrength);
    glUniform1f(glGetUniformLocation(m_shader, "uWindFrequency"), config.windFrequency);
    glUniform3fv(glGetUniformLocation(m_shader, "uColorBase"), 1, glm::value_ptr(config.colorBase));
    glUniform3fv(glGetUniformLocation(m_shader, "uColorTip"),  1, glm::value_ptr(config.colorTip));

    // Grass needs double-sided rendering
    glDisable(GL_CULL_FACE);

    glBindVertexArray(m_VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            m_indexCount,
                            GL_UNSIGNED_INT,
                            nullptr,
                            m_instanceCount);
    glBindVertexArray(0);

    // Restore cull face if your engine uses it
    glEnable(GL_CULL_FACE);

    glUseProgram(0);
}

// ============================================================
//  Shutdown
// ============================================================
void GrassSystem::Shutdown()
{
    if (!m_ready) return;
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_bladeVBO);
    glDeleteBuffers(1, &m_instanceVBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteProgram(m_shader);
    m_ready = false;
}