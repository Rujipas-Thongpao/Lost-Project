#version 330 core
out vec4 FragColor;
  
in vec3 Normal;
in vec2 TexCoord;
in vec3 PositionWS;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ViewDirWS;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

void main()
{    
    // ambient
    vec3 ambient = lightColor * material.ambient;
  	
    // diffuse 
    // vec3 norm = normalize(texture(texture_normal1, TexCoord)).rgb;
    vec3 lightDir = normalize(lightPosition);
    float ndotl= max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = lightColor * ndotl;
    
    // specular
    vec3 viewDir = normalize(ViewDirWS - PositionWS);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specularMap = texture(texture_specular1, TexCoord).rgb;
    vec3 specular = lightColor * (spec * material.specular);  
        
    vec3 result = diffuse;
    vec3 baseTex = texture(texture_diffuse1, TexCoord).rgb;
    result *= baseTex;

    FragColor = vec4(result, 1.0);
}
