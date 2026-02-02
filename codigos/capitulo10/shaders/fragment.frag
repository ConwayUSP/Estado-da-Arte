#version 430 core

struct Luz {
    vec3 posicao;
    vec3 ambiente;
    vec3 difuso;
    vec3 especular;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 FragColor;

uniform vec3 visaoPos;
uniform Luz luz;
uniform sampler2D textura_difusa1;
uniform sampler2D textura_AO1;
uniform sampler2D textura_rugosa1;
uniform sampler2D textura_metal1;

void main() {
    // componente ambiente
    vec3 ambiente = luz.ambiente * texture(textura_difusa1, texCoords).rgb;

    // componente difuso
    vec3 normal = normalize(normal);
    vec3 luzDir = normalize(luz.posicao - fragPos);
    float dif = max(dot(normal, luzDir), 0.0);
    vec3 difuso = luz.difuso * dif * texture(textura_difusa1, texCoords).rgb;

    vec3 cor = ambiente + difuso;
    FragColor = vec4(cor, 1.0);
}
