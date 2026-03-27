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

(...)
