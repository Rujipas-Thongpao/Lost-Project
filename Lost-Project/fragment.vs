#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Normal;
in vec3 PositionWS;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ViewDirWS;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

uniform vec3 u_camPos;
uniform float u_ditherFadeStart; // e.g. 3.0
uniform float u_ditherFadeEnd;   // e.g. 1.0  (closer = fully discarded)

float bayer4x4(ivec2 pos) {
    const int bayer[16] = int[16](
         0,  8,  2, 10,
        12,  4, 14,  6,
         3, 11,  1,  9,
        15,  7, 13,  5
    );
    return float(bayer[(pos.y % 4) * 4 + (pos.x % 4)]) / 16.0;
}

void applyDitherFade(vec3 worldPos) {
    float dist = length(u_camPos - worldPos);
    // 1.0 at fadeStart (fully visible), 0.0 at fadeEnd (fully discarded)
    float alpha = clamp((dist - u_ditherFadeEnd) / (u_ditherFadeStart - u_ditherFadeEnd), 0.0, 1.0);
    float threshold = bayer4x4(ivec2(gl_FragCoord.xy));
    if (alpha < threshold) discard;
}

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

void main()
{    
    applyDitherFade(PositionWS);
    // ambient
    vec3 ambient =  vec3(.57, 0.72, 0.76) * 0.4;
  	
    // diffuse 
    // vec3 norm = normalize(texture(texture_normal1, TexCoord)).rgb;
    vec3 lightDir = normalize(lightPosition);
    float ndotl= max(dot(Normal, lightDir), 0.0);
    float step = 3.0;
    float ndotl_toon = round(ndotl * step)/step;
    vec3 diffuse = lightColor * ndotl_toon;
    
    // specular
    vec3 viewDir = normalize(ViewDirWS - PositionWS);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
    vec3 specular = lightColor * spec;  
        
    vec3 result = diffuse + specular + ambient;
    vec3 baseTex = texture(texture_diffuse1, TexCoord).rgb;
    result *= baseTex;



    FragColor = vec4(result, 1.0);
}
