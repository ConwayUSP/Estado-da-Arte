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

## Armazenando e Misturando Cores

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

## Iluminando Uma Cena

Para criar a iluminação de uma cena, a primeira coisa que precisamos é de uma fonte de luz. Aqui, para fins de simplificação, ela será um cubo. Além disso, também usaremos um dos benditos contêiner de cubo.

Inicialmente, vamos precisar de um vertex shader para desenhar o contêiner. As posições dos vértices do contêiner devem continuar as mesmas, então nenhuma novidade por aqui.

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

Já que queremos renderizar uma fonte de luz na forma de um cubo, vamos querer gerar um novo VAO especificamente para a fonte de luz:

```cpp
unsigned int lightVAO;
glGenVertexArrays(1, &lightVAO);
glBindVertexArray(lightVAO);
// Precisamos apenas fazer o binding com o VBO, os dados do VBO do contêiner
// já contêm os dados.
glBindBuffer(GL_ARRAY_BUFFER, VBO);
// set the vertex attribute
// Definindo o atributo do vértice
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),(void*)0);
glEnableVertexAttribArray(0);
```

Ok! Agora que criamos os cubos, vamos definir o shader de fragmento para ambos:

```glsl
#version 430 core
out vec4 FragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
void main(){
    FragColor = vec4(lightColor * objectColor, 1.0);
}
```

O shader de fragmento aceita uma cor de objeto e de luz de uma variável uniforme. Aqui, novamente, nós multiplicamos a cor da luz com a do objeto. 

Vamos definir a cor do objeto como sendo a mesma do coral e da luz como sendo branca

```glsl
lightningShader.use();
lightningShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
lightningShader.setVec3("lightColor", 1.0f, 1.00f, 1.00f);
```

Em um projeto real, conforme formos ajustando os shaders de iluminação no avanço do nosso código, o nosso querido cubo luminoso seria afetado, e isso não é algo que queremos. Aqui, queremos que ele mantenha uma cor e um brilho constante.

Para isso, vamos criar um outro conjunto de shaders que usaremos para desenhar a nossa fonte de luz e manter ela segura de eventuais mudanças nos shaders de iluminação.

O vertex shader que vamos criar é o mesmo que o vertex shader de iluminação, então é só dar copy-paste do código:

```glsl
#version 430 core
out vec4 FragColor;

void main(){
    FragColor = vec4(1.0);
}
```

Quando quisermos renderizar, vamos querer rederizar o objeto contêiner usando o shader de iluminação que acabamos de definir. Quando quisermos desenhar a fonte de luz, usaremos os shaders da fonte de luz em si. 

A ideia do cubo é só mostrar de onde a luz da cena vem. Por isso, renderizamos ele na mesma posição que a fonte de luz. 

Então, vamos declarar um `vec3` global para representar a localização da fonte de luz nas coordenadas do word-space:

```c++
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
```

E, depois, transladar o cubo de fonte de luz para a posição da fonte de luz em si e encolhê-lo antes de renderizar:

```c++
model = glm::mat4(1.0f);
model = glm::translate(model, lightPos);
model = glm::scale(model, glm::vec3(0.2f);
```

O código de renderização final deve ficar mais ou menos assim:

```cpp
lightCubeShader.use();
// Nas próximas linhas, defina o modelo e a matriz de visão e de projeção
// (...)
// Desenhando o objeto "cubo luminoso" 
glBindVertexArray(lightCubeVAO);
glDrawArrays(GL_TRIANGLES, 0, 36);
```

Enfim. Desenvolvendo o seu código em `C++` para OpenGL corretamente, compilando e rodando, teremos o seguinte:

(Imagem aqui)

Qualquer dúvida, não deixe de consultar o código completo na pasta de códigos da nossa trilha!

## Iluminação Básica

O funcionamento da luz no mundo real é extremamente complexo e existem diversos fatores que podem alterar a sua disposição e se alcance até nossas retinas. Obviamente, precisamos nos limitar a certas coisas específicas pois, normalmente, não possuímos poder computacional ilimitado ou algo nesse sentido.

A iluminação no OpenGL é baseada em modelos simplificados, mas que produzem resultados bem realistas e interessantes para nós. 

Um desses modelos é o `Modelo de Iluminação de Phong` e a construção a partir desse pilar consiste em três componentes: _iluminação ambiente, difusa e especular_.

![Cubos com exemplos de iluminação](../imagens/07_combinaphong.png)

> Imagem retirada do livro Learn OpenGL. Nela, você consegue perceber como cada uma das componentes atua individualmente e como fica quando todas são combinadas.

i) Iluminação Ambiente: no mundo real, mesmo quando estamos em um local bem escuro, sempre existe algum resquício de iluminação, então os objetos nunca estão completamente escuros. Para simular isso, vamos usar uma _iluminação ambiente_ que sempre dê alguma cor para os objetos.

ii) Iluminação Difusa: simula o impacto direto da luz em um objeto.

iii) Iluminação Especular: simula o ponto de uma luz que aparece em objetos brilhantes.

### Luz Ambiente

Adicionar uma iluminação ambiente no nosso projeto é bem simples. Nós pegamos a cor da luz, multiplicamos ela por uma constante pequena de "fator ambiente", multiplicamos pela cor do objeto e usamos isso como a cor do fragmento no shader do cubo do objeto. Em outras palavras:

```glsl
void main(){
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
}
```

Ao rodar o programa, você vai perceber que já estará funcionando e terá mais ou menos algo nesse sentido:

(imagem)
> O objeto está meio escuro, mas não completamente. Além disso, o cubo de luz não foi afetado (por quê?).

### Luz Difusa

Se a primeira aplicação de iluminação não te animou tanto, é aqui que começamos a ter resultados mais interessantes. 

A iluminação difusa dá ao objeto maior clareza conforme os fragmentos são mais alinhados com os raios da fonte de luz. Para você ter uma noção melhor, observe a seguinte imagem:

(imagem)

À esquerda, encontramos uma fonte de iluminação com um raio de luz coincidindo com um ponto específico do nosso objeto. No caso, é necessário estimar o ângulo de incidência para definir a iluminação final. Se o raio de luz é perpendicular, tal como um solzão de meio-dia, temos a maior quantidade de reflexão possível. 

Para mensurar esse ângulo, utilizamos uma coisa chamada de _vetor normal_, que é um vetor perpendicular à superfície do fragmento. O ângulo entre os dois vetores pode ser calculado usando o produto escalar. Caso você não se lembre a respeito do produto escalar, nós abordamos ele no nosso [capítulo de revisão de conceitos matemáticos](https://github.com/ConwayUSP/Estado-da-Arte/blob/main/capitulos/04%20-%20Matematica.md). Vale à pena dar uma conferida.

> Perceba que, para conseguirmos o cosseno do ângulo entre dois vetores, trabalharemos com vetores unitários (vetores com comprimento igual a 1). Então, precisamos ter certeza de que todos os vetores estejam normalizados, se não o produto escalar vai retornar valores que vão além do cosseno.

O produto escalar resultante vai retornar que usaremos para calcular o impacto da luz na cor do fragmento, fazendo com que tenhamos resultados diferentes em função dos diferentes posicionamentos do objeto diante da fonte de luz.

Então, para calcular isso, vamos precisar do _vetor normal_ e do raio de luz direcionado. Para este último, um vetor de direção que é um vetor resultante da diferença a posição da luz e do fragmento.

### Vetores Normais

Conforme acabamos de falar, um vetor normal é um vetor (unitário neste caso) que é perpendicular à superfície de um vértice. Mas, ora, um vértice é só um ponto, então não tem uma "superfície", certo? Sim, por isso vamos pegar um vetor normal utilizando os vértices ao redor dele para definir essa superfície.

Podemos calcular os vetores normais para todos os vértices do cubo ao utilizar o produto vetorial (caso você não lembre, novamente indicamos o [Capítulo 4](https://github.com/ConwayUSP/Estado-da-Arte/blob/main/capitulos/04%20-%20Matematica.md) desta trilha).

Porém, já que um cubo não é uma forma muito complicada, podemos adicionar eles manualmente (ou não) nos dados do vertex. Observe (e não se assuste):

```cpp
// Dados dos vértices
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
};
```
```glsl
// Vertex Shader
out vec3 Normal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = aNormal;
}
```

Assim, o que falta fazer é declarar a variável de entrada no fragment shader:

```glsl
in vec3 Normal;
```

### Calculando a Cor Difusa

Agora que temos o vetor normal para cada vértice, precisamos da posição da luz e do fragmento, ambas na forma de vetores.

Declarando a posição da luz como um uniform no fragment shader:

```glsl
uniform vec3 lightPos;
```

E, também, atualizar o uniform no loop de renderização (por quê?). Vamos utilizar o `lightPos` declarado no capítulo passado como sendo a fonte da luz difusa:

```cpp
lightningShader.setVec3("lightPos", lightPos);
```

Por fim, precisamos da posição atual do fragmento. 

### Um detalhe a mais

### Luz especular

## Conclusão
