#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 PositionWS;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 totalPosition = vec4(0.0f);
	vec3 totalNormal = vec3(0.0f);

	totalPosition = vec4(aPos,1.0f);
	totalNormal = aNormal;
	
    TexCoord = texCoord;
    Normal = mat3(transpose(inverse(model))) * totalNormal;  
    gl_Position = projection * view * model * totalPosition;
    PositionWS = vec3(model * totalPosition);
}
