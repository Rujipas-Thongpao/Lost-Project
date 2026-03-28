#version 430 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal; // the normal variable has attribute position 1
layout (location = 2) in vec2 texCoord; // the position variable has attribute position 0
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

out vec2 TexCoord; // output texture coordinate to the fragment shader
out vec3 Normal; // output normal to the fragment shader
out vec3 PositionWS;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool isAnimated;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];


void main()
{
    vec4 totalPosition = vec4(0.0f);
	vec3 totalNormal = vec3(0.0f);
    if(isAnimated){
		for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
		{
			if(boneIds[i] == -1) 
				continue;
			if(boneIds[i] >=MAX_BONES) 
			{
				totalPosition = vec4(aPos,1.0f);
				break;
			}
			vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
			totalPosition += localPosition * weights[i];
			totalNormal += mat3(transpose(inverse(finalBonesMatrices[boneIds[i]]))) * aNormal * weights[i];
		}
    }
	else{
		totalPosition = vec4(aPos,1.0f);
		totalNormal = aNormal;
	}

	// TODO : Have to fix normal
	totalNormal = aNormal;
	
    TexCoord = texCoord;
    Normal = mat3(transpose(inverse(model))) * totalNormal;  
    gl_Position = projection * view * model * totalPosition;
    PositionWS = vec3(model * totalPosition);
}
