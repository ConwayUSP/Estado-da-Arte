# Iluminação Básica

Uma das coisas que mais possuem potencial para alterar a maneira a qual compreendemos um projeto de computação gráfica é, justamente, a iluminação da cena em questão. A percepção individual de todos os objetos ali presentes gira em torno disso, refletindo como interpretaremos o mundo.

Tendo em vista esse ponto, neste capítulo nós veremos:

1. Armazenando e misturando cores
2. Criando um objeto fonte de luz
3. Descrição do modelo Blinn-Phong
4. Componente ambiente
5. Componente difuso
6. Componente especular e cuidados com vetores normais
7. Definindo materiais para o modelo Phong
8. Modificando as propriedades da fonte de luz

## Armazenando e misturando cores

Na sua infância (ou qualquer outra fase da sua vida), possivelmente você brincou com tintas. O que acontecia quando você misturava algumas? Uma "nova" cor surgia, certo? Além disso, dependendo da quantidade de cada tinta que você colocasse na mistura, a tonalidade da nova cor era alterada.

Então, eis aqui uma oportunidade de voltar a brincar com cores.

Como seria possível fazer isso no computador? 

No mundo digital precisamos fazer o mapeamento de valores contínuos (infinitos) de cores para os discretos. Além disso, nós teremos três "tintas" básicas para chegar em praticamente qualquer outra cor. No caso, o vermelho (Red), o verde (Green) e o azul (Blue), os quais foram o famoso _RGB_. A "quantidade de cada tinta" que vamos usar fica dentro de um intervalo [0, 1]. Com base nisso, podemos definir um vetor de cores da seguinte maneira com GLM:

```cpp
glm::vec3 coral(1.0f, 0.5f, 0.31f);
```

Na vida real, a cor de um objeto o qual visualizamos nada mais é do que a cor que ele reflete. Parte do espectro de cores é absorvida e o resto é refletido. 

(Imagem aqui)

Como o espectro de cores que coincide no nosso objeto influencia diretamente na cor que percebemos dele, precisamos representar isso de alguma maneira no computador. Fazemos isso multiplicando componente por componente do vetor de luz e do vetor que representa a cor do objeto:

```cpp
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
glm::vec3 result = lightColor * toyColor; // = (1.0f, 0.5f, 0.31f);
```

Acima, no vetor de luz, utilizamos as componentes que combinadas formam a luz branca. Ocorreu, então, uma reflexão fiel da cor do objeto. 

Agora, o que aconteceria se usássemos uma cor verde?

```cpp
glm::vec3 lightColor(0.0f, 1.0f, 0.0f); // Todas as componentes zeradas, exceto a verde
glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
glm::vec3 result = lightColor * toyColor; // = (0.0f, 0.5f, 0.0f);
```

No caso, não existiria cor vermelha ou azul para ser refletida, resultando apenas na reflexão da componente verde, que é inteiramente preservada.

Mas, e se nós usássemos alguma cor mais específica, como verde oliva:

```cpp
glm::vec3 lightColor(0.33f, 0.42f, 0.18f);
glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
glm::vec3 result = lightColor * toyColor; // = (0.33f, 0.21f, 0.06f);
```

Enfim. Podemos fazer um objeto refletir diferentes tonalidades de cor a partir do uso de diferentes iluminações.

## Iluminando uma cena

Para criar a iluminação de uma cena, a primeira coisa que precisamos é de uma fonte de luz. Aqui, para fins de simplificação, ela será um cubo. Além disso, também usaremos um dos benditos container de cubo.

Inicialmente, vamos precisar de um vertex shader para desenhar o container. As posições dos vértices do container devem continuar as mesmas, então nenhuma novidade por aqui.

```glsl
#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
```
