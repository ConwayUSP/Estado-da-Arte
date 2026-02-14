#version 430 core

struct Material {
    sampler2D difuso;
    sampler2D ARM;
    float brilhosidade;
};

struct Luz {
    vec3 posicao;
    vec3 direcao;
    float abertura;
    float arcoExterno;
    vec3 ambiente;
    vec3 difuso;
    vec3 especular;
};

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 FragColor;

uniform vec3 visaoPos;
uniform Material material;
uniform Luz luz;

vec3 CalculaLuzCeu(vec3 normal)
{
    vec3 luzDir = vec3(0.0, 1.0, 0.0);
    float dif = max(dot(normal, luzDir), 0.0);
    return (dif * vec3(0.05, 0.1, 0.25));
}

vec3 CalculaLuzPonto(vec3 normal) {
    vec3 corLuz = vec3(0.4, 0.1, 0.25);
    vec3 luzPos = vec3(5.0, 2.0, 6.0);
    vec3 luzDir = normalize(luzPos - fragPos);
    vec3 reflexoDir = reflect(-luzDir, normal);
    float dif = max(dot(normal, luzDir), 0.0);
    float espec = pow(max(dot(luzDir, reflexoDir), 0.0), material.brilhosidade);
    // atenuação
    float distancia = length(luzPos - fragPos);
    float atenuacao = 1.0 / (1.0 + 0.02 * distancia + 0.005 * (distancia * distancia));
    // resultado
    vec3 difuso = corLuz * dif * vec3(texture(material.difuso, texCoords));
    vec3 especular = corLuz * espec * (1 - texture(material.ARM, texCoords).g);
    return atenuacao * (difuso + especular);
}

void main()
{
    vec3 arm = texture(material.ARM, texCoords).rgb;
    float ao = arm.r;
    float roughness = arm.g;
    float metal = arm.b;
    vec3 luzDir = normalize(luz.posicao - fragPos);
    float angulo = dot(luzDir, normalize(-luz.direcao));
    float cosDif = luz.abertura - luz.arcoExterno;
    float intensidade = clamp((angulo - luz.arcoExterno) / cosDif, 0.0, 1.0);
    float distLanterna = length(fragPos - luz.posicao);
    float atenuacaoLanterna = 1.0 / (1.0 + 0.05 * distLanterna + 0.02 * distLanterna);
    intensidade *= atenuacaoLanterna;

    // componente ambiente
    vec3 ambiente = luz.ambiente * texture(material.difuso, texCoords).rgb * ao;

    // partes do componente difuso
    vec3 normal = normalize(normal);
    float dif = max(dot(normal, luzDir), 0.0);
    // partes do componente especular
    vec3 reflexoDir = reflect(-luzDir, normal);
    vec3 visaoDir = normalize(visaoPos - fragPos);
    float espec = pow(max(dot(visaoDir, reflexoDir), 0.0), material.brilhosidade);
    // componente da luz de ponto
    vec3 luzPonto = CalculaLuzPonto(normal);
    // componente da luz direcional
    vec3 luzCeu = CalculaLuzCeu(normal) * dif * ao * (1 - metal);

    if (angulo > luz.arcoExterno) {
        // componente difuso
        vec3 difusoLant = texture(material.difuso, texCoords).rgb;
        difusoLant *= luz.difuso * dif * ao * (1 - metal);
        // componente especular
        vec3 especularLant = luz.especular * espec * (1 - roughness);
        // resultado
        vec3 cor = ambiente + luzCeu + luzPonto + intensidade * (difusoLant + especularLant);
        FragColor = vec4(cor, 1.0);
    } else {
        vec3 cor = ambiente + luzCeu + luzPonto;
        FragColor = vec4(cor, 1.0);
    }
}
