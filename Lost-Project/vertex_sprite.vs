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

void main() {
    // extract camera axes from view matrix
    vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp    = vec3(view[0][1], view[1][1], view[2][1]);

    // particle center in world space
    vec3 worldPos = vec3(model[3]);

    // offset vertex along camera axes — quad always faces camera
    vec3 vertexPos = worldPos
        + cameraRight * aPos.x
        + cameraUp    * aPos.y;

    PositionWS  = vertexPos;
    TexCoord    = texCoord;
    gl_Position = projection * view * vec4(vertexPos, 1.0);
}
