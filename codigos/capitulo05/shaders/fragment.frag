#version 430 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D textura1;
uniform sampler2D textura2;

void main() {
    FragColor = mix(texture(textura1, TexCoord), texture(textura2, TexCoord), 0.2);
}