#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTex;

float luminance(vec3 col) {
    return dot(col, vec3(0.299, 0.587, 0.114));
}

void main() {
    vec2 texelSize = vec2(1.0/800.0, 1.0/800.0);
    // Sample 3x3 neighborhood
    float tl = luminance(texture(screenTex, TexCoords + texelSize * vec2(-1,  1)).rgb);
    float t  = luminance(texture(screenTex, TexCoords + texelSize * vec2( 0,  1)).rgb);
    float tr = luminance(texture(screenTex, TexCoords + texelSize * vec2( 1,  1)).rgb);
    float l  = luminance(texture(screenTex, TexCoords + texelSize * vec2(-1,  0)).rgb);
    float r  = luminance(texture(screenTex, TexCoords + texelSize * vec2( 1,  0)).rgb);
    float bl = luminance(texture(screenTex, TexCoords + texelSize * vec2(-1, -1)).rgb);
    float b  = luminance(texture(screenTex, TexCoords + texelSize * vec2( 0, -1)).rgb);
    float br = luminance(texture(screenTex, TexCoords + texelSize * vec2( 1, -1)).rgb);

    // Sobel kernels
    float gx = -tl - 2.0*l - bl + tr + 2.0*r + br;
    float gy = -tl - 2.0*t - tr + bl + 2.0*b + br;

    float edge = sqrt(gx*gx + gy*gy);

    // Composite: original color + black outline
    vec3 sceneColor = texture(screenTex, TexCoords).rgb;
    float outline = smoothstep(0.4, 0.9, edge); // tweak thresholds
    vec3 outlineColor = vec3(19.0/255.0,20.0/255.0,29.0/255.0);               

    FragColor = vec4(mix(sceneColor, outlineColor, outline), 1.0);
}