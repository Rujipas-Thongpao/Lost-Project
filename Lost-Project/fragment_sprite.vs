#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Normal;
in vec3 PositionWS;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ViewDirWS;
uniform sampler2D mainTex;
uniform vec4 color;

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
    vec3 lightDir = normalize(lightPosition);
    float ndotl= max(dot(Normal, lightDir), 0.0);

    // toon
    float step = 3.0;
    float ndotl_toon = round(ndotl * step)/step;

    vec3 diffuse = lightColor * ndotl_toon;
    
    // specular
    vec3 viewDir = normalize(ViewDirWS - PositionWS);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);  
        
    vec3 result = diffuse;
    vec4 baseTex = texture(mainTex, TexCoord);
    result *= baseTex.rgb;

    if(baseTex.a < 0.1) 
		discard;

    vec3 col = baseTex.rgb;
    col *= color;

    FragColor = vec4(col, 1.0);
}
