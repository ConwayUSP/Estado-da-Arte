# Texturas

Se você já jogou algum jogo antigo ou um título indie em desenvolvimento, provavelmente já se deparou com o famoso "quadrado roxo e preto" (o pesadelo de todo jogador de Garry's Mod ou Counter-Strike). Isso acontece quando o motor gráfico tenta aplicar uma **textura** e não a encontra. Até agora, nós pintamos nossos triângulos com cores sólidas ou degradês básicos, mas para dar realismo e detalhes (como madeira, pedra ou o rosto de um personagem), precisamos das **texturas**.

!!!Pegar imagem: https://pt.egw.news/gaming/news/28885/garrys-mod-fixes-missing-textures-with-built-in-cs-mwhMxySpP

Neste capítulo, vamos transformar nossos triângulos sem graça em objetos que realmente pareçam ter uma superfície.

Aqui está o que vamos ver:

1. O que é uma Textura
2. Coordenadas de Textura (UVs)
3. Texture Wrapping (O que acontece nas bordas)
4. Texture Filtering (Pixels vs. Suavização)
5. Mipmaps
6. Carregando imagens com `stb_image`
7. Criando e configurando Objetos de Textura
8. Usando Texturas nos Shaders
9. Texture Units (Usando mais de uma imagem)

## O que é uma Textura

Uma **textura** é basicamente uma imagem que você "cola" em cima de uma geometria. Pense assim: você tem uma caixa de papelão, e quer que ela pareça uma caixa de madeira. Em vez de modelar cada veio e nó da madeira com vértices (o que seria uma loucura), você simplesmente imprime uma foto de madeira e cola por fora. É exatamente isso que o OpenGL faz com texturas.

Além de imagens visuais, texturas também podem armazenar dados arbitrários para serem enviados aos shaders: normal maps, height maps, e outros recursos que veremos mais para frente.

!!!Colocar imagem do triangulo com tijolos

## Coordenadas de Textura

Para o OpenGL saber qual parte da imagem vai em qual parte do triângulo, usamos as coordenadas de textura. Elas variam de $0.0$ a $1.0$ nos eixos $x$ e $y$ (que no contexto de texturas chamamos de $s$ e $t$).

O ponto $(0,0)$ é o canto inferior esquerdo da imagem, e $(1,1)$ é o canto superior direito. Se você tem um triângulo e quer que ele exiba a imagem inteira, você atribui $(0,0)$ ao vértice inferior esquerdo, $(1,0)$ ao inferior direito e $(0.5, 1.0)$ ao topo.

!!!Imagem do triangulo com os cantos rotulados

No capítulo anterior, cada vértice tinha posição (3 floats) + cor (4 floats) = 7 floats. A partir de agora, como as cores virão da textura, vamos trocar o atributo de cor pelas coordenadas UV (2 floats). Cada linha do array de vértices passa a ter 5 floats:

No array de vértices, cada linha ficaria algo assim:

```cpp
float vertices[] = {
    // posicoes         // coords de textura
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // inferior direito
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // inferior esquerdo
     0.0f,  0.5f, 0.0f,  0.5f, 1.0f   // topo
};
```

### Atualizando o `glVertexAttribPointer`:

Como mudamos a estrutura dos vértices, precisamos atualizar as chamadas ao glVertexAttribPointer. O stride agora é 5 * sizeof(float) e removemos o atributo de cor (location 1), substituindo-o pelo de coordenada de textura:

```cpp
// Atributo de posição (location = 0) — sem mudança na lógica, só no stride
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Atributo de coordenada de textura (location = 1)
// -> 2 componentes (s e t)
// -> stride de 5 * sizeof(float) = 20 bytes
// -> offset de 3 * sizeof(float) = 12 bytes (pula os 3 floats de posição)
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

### Atualizando os Shaders:

Com a mudança nos atributos, os shaders também precisam ser atualizados. No Vertex Shader, trocamos o aCor pelo aTexCoord e passamos as coordenadas adiante para o Fragment Shader:

```
// shaders/vertex.vert
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord; // <-- era aCor vec4, agora é UV vec2

out vec2 TexCoord; // passa para o fragment shader

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
```

No Fragment Shader, recebemos as coordenadas e... por enquanto, deixamos ele parado, pois precisamos entender mais algumas coisas antes de usar uma textura de verdade:

```
// shaders/fragment.frag
#version 430 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D textura1; // veremos isso em breve!

void main() {
    FragColor = texture(textura1, TexCoord);
}
```
O tipo sampler2D e a função texture() vão fazer sentido nas próximas seções. Por enquanto, só registre que é assim que o Fragment Shader acessa uma textura.

## Texture Wrapping

O que acontece se a gente definir uma coordenada como $(2.0, 2.0)$? O OpenGL precisa de uma regra para saber o que desenhar fora do intervalo $[0, 1]$. As opções mais comuns são:

- `GL_REPEAT`: A textura se repete indefinidamente (padrão).
- `GL_MIRRORED_REPEAT`:  Repete, mas espelha a imagem a cada repetição.
- `GL_CLAMP_TO_EDGE`: Qualquer valor fora de [0,1] é travado na cor da borda. O pixel da borda se estica até o infinito.
- `GL_CLAMP_TO_BORDER`: O que estiver fora ganha uma cor sólida definida por você.

!!! Imagem dos 4 modelos de wrapping

Você configura isso por eixo separadamente, usando `glTexParameteri`:

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
```

Se você escolher `GL_CLAMP_TO_BORDER`, pode definir a cor da borda assim:

```cpp
float corDaBorda[] = { 1.0f, 0.5f, 0.0f, 1.0f }; // laranja
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, corDaBorda);
```

## Texture Filtering

As texturas têm uma resolução fixa, mas os triângulos na tela podem ter qualquer tamanho. O Texture Filtering decide como o OpenGL calcula a cor de um pixel quando a textura precisar ser ampliada (magnification) ou reduzida (minification).

1. `GL_NEAREST` (Nearest Neighbor): Escolhe o pixel da textura mais próximo da coordenada UV. O resultado é um visual pixelado e nítido. É o preferido para jogos de Pixel Art e é mais rápido computacionalmente.

2. `GL_LINEAR` (Bilinear):  Faz uma média ponderada entre os 4 pixels mais próximos. O resultado é um visual mais suave, mas pode parecer levemente embaçado de perto.

!!! Colocar imagem de exemplo que está no site

Você configura o filtering separadamente para cada situação:
```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // ao reduzir
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // ao ampliar
```

## Mipmaps

Imagine um plano com uma textura de alta resolução muito longe da câmera. Tentar renderizar isso causa o efeito de *aliasing* (aqueles pontinhos brancos que ficam "sambando" na tela). Para resolver isso, o OpenGL usa **Mipmaps**.

**Mipmaps** são basicamente versões menores da sua textura (metade do tamanho, depois um quarto, e assim por diante) que o OpenGL gera para usar quando o objeto está longe.

!!!COlocar imagem dos níveis de mipmap

A boa notícia é que você não precisa criar essas versões manualmente:

```cpp
glGenerateMipmap(GL_TEXTURE_2D);
```

Para o filtering entre os níveis de mipmap, usamos constantes combinadas:

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
```

## Carregando imagens com stb_image.h

O OpenGL não sabe ler arquivos .jpg ou .png. Ele espera um array de bytes com os dados brutos de cor de cada pixel. Para não escrever um decodificador de JPEG na mão (o que é um convite a um colapso nervoso), usamos a biblioteca `stb_image`

Ela é uma biblioteca header-only: basta baixar o arquivo stb_image.h e colocá-lo na nossa pasta src/modules/. Depois, crie um arquivo src/modules/stb_image.cpp com apenas essas duas linhas:

```cpp
// src/modules/stb_image.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
```

O #define diz para a biblioteca incluir sua implementação neste arquivo. Nos demais arquivos do projeto (como o main.cpp), basta incluir o header normalmente, sem o `define`.

Agora adicione o include no topo do main.cpp:

Depois, carregar uma imagem é moleza:

```cpp
int largura, altura, nCanais;
unsigned char *dados = stbi_load("texturas/XXXXXXXX", &largura, &altura, &nCanais, 0);
```

A função retorna um ponteiro para os pixels e preenche as variáveis com a largura, altura e número de canais (3 para RGB, 4 para RGBA com transparência).

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

## Criando a Textura



## Usando no Shader



## Conclusão

Texturas dão vida ao cenário. Sem elas, estaríamos presos a um mundo de cores sólidas e matemáticas. O segredo aqui é entender o mapeamento UV e como o OpenGL lida com a memória da GPU. Se sua textura aparecer preta, 90% das vezes é porque você esqueceu o glBindTexture ou o caminho do arquivo estava errado.

```
(0,1) T _____________________ (1,1)
      ^ |                   |
      | |      _\|/_        |
      | |      (o o)        |
      | |---oOO-{_}-OOo---  |
      | | Que suas UVs nunca|
      | |   se sobreponham  |
      | |-------------------|
      +-------------------> S
(0,0)                       (1,0)
```

## Exercícios propostos

1. Mude as coordenadas UV do triângulo para valores acima de 1.0 (ex: 2.0) e experimente os 4 modos de wrapping.
2. 
