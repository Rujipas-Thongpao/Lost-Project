#version 430 core
in  vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D screenTexture;

in vec2 TexCoords;

void main() {
    FragColor = texture(screenTexture, TexCoords);
}