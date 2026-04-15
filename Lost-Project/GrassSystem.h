#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

// ============================================================
//  GrassSystem
//  Self-contained GPU-instanced grass renderer.
//  Does NOT use the ECS – call Init() once, Render() every frame.
// ============================================================

struct GrassConfig
{
    // Field layout
    int   countX = 200;       // blades along X
    int   countZ = 200;       // blades along Z
    float spacingX = 0.25f;     // metres between blades (X)
    float spacingZ = 0.25f;     // metres between blades (Z)
    float originX = -25.0f;    // world-space start X
    float originZ = -25.0f;    // world-space start Z

    // Blade geometry
    float bladeWidth = 0.08f;     // half-width at base
    float bladeHeight = 0.9f;      // base height (noise adds to this)
    int   segments = 4;         // vertical segments per blade (min 2)

    // Noise-driven height variation (high frequency, per-blade jitter)
    float noiseScale = 0.15f;     // world-space frequency of height noise
    float noiseAmp = 0.45f;     // ± metres of height variation

    // Density mask — Perlin fBm at low frequency shapes where grass grows
    float densityNoiseScale = 0.04f;  // lower = broader meadow patches
    float densityThreshold = 0.40f;  // 0=grass everywhere, 0.7=sparse patches
    int   densityOctaves = 4;       // more octaves = raggier patch edges

    // Wind animation
    float windSpeed = 1.2f;
    float windStrength = 0.18f;
    float windFrequency = 0.8f;    // spatial frequency of wind wave

    // Colors
    glm::vec3 colorBase = glm::vec3(135.0f, 201.0f, 99.0f)/ 255.0f;
    glm::vec3 colorTip = glm::vec3(184.0f, 242.0f, 77.0f) / 255.0f;
};

class GrassSystem
{
public:
    GrassConfig config;

    void Init();
    void Init(const GrassConfig& cfg);

    // Call after your main scene render, before ImGui.
    // view/proj come from your CameraSystem.
    void Render(const glm::mat4& view, const glm::mat4& proj, float time);

    void Shutdown();

    ~GrassSystem() { Shutdown(); }

private:
    // ---- OpenGL objects ----
    GLuint m_VAO = 0;
    GLuint m_bladeVBO = 0;  // per-blade vertex data
    GLuint m_instanceVBO = 0;  // per-instance data (world XZ pos + random seed)
    GLuint m_EBO = 0;
    GLuint m_shader = 0;

    int    m_indexCount = 0;
    int    m_instanceCount = 0;

    bool   m_ready = false;

    // ---- helpers ----
    void buildBladeMesh(std::vector<float>& verts, std::vector<unsigned int>& indices);
    void buildInstanceData(std::vector<float>& data);
    GLuint compileShader(GLenum type, const char* src);
    GLuint linkProgram(GLuint vert, GLuint frag);

    // Simple value noise used CPU-side for height jitter
    float valueNoise(float x, float z);
    // Gradient (Perlin) noise — used for density mask
    float perlinNoise(float x, float z);
    // Fractional Brownian Motion — layered Perlin octaves, returns [0,1]
    float fbm(float x, float z, int octaves = 4,
        float lacunarity = 2.0f, float gain = 0.5f);
};