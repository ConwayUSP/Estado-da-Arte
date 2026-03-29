# Exercícios Propostos

## Exercício 1

Exemplos de substituições possíveis e seus resultados:

```cpp
// Alteração para o uso de GL_POINTS
glDrawArrays(GL_POINTS, 0, 3);

// Abaixo faço uso da função glPointSize para auxiliar na visulização do ponto na imagem
glPointSize(5.0f);
```

![triangulo com vertices roxo](../imagens/01_triangulo_vertices_roxo.png)

```cpp
// Alteração para o uso de GL_LINE_LOOP
glDrawArrays(GL_LINE_LOOP, 0, 3);
```

![triangulo com arestas roxas](../imagens/01_triangulo_arestas_roxo.png)

## Exercício 2

Com adição da função `glPolygonMode` o código e a imagem ficarão assim:

```cpp
// Define que tanto a frente quanto o verso do polígono serão desenhados apenas com linhas
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

![triangulo com arestas roxas](../imagens/01_triangulo_arestas_roxo.png)

Perceba que visualmente a ultilização do parâmetro `GL_LINE_LOOP` e da função `glPolygonMode` com `GL_LINE` resultou no mesmo triângulo somente com as arestas presentes, porém há diferenças na lógica por trás deles:

- `GL_LINE_LOOP`: É uma primitiva de desenho. Você está dizendo ao OpenGL: "Esqueça triângulos, pegue estes pontos e conecte-os, fechando o último no primeiro". O OpenGL nem chega a processar esse objeto como uma superfície sólida e, desse modo, se você passar os 4 vértices de um quadrado, ele desenhará apenas o contorno externo.

- `glPolygonMode(..., GL_LINE)`: É uma configuração de estado. Você continua enviando triângulos (`GL_TRIANGLES`), e o OpenGL continua calculando tudo o que um triângulo exige, mas, na hora de "pintar" os pixels na tela, ele é instruído a desenhar apenas as bordas em vez de preencher o interior. Dessa forma, se você usar essa função nesse modo em um quadrado feito por dois triângulos, o OpenGL desenhará o contorno externo e a linha diagonal que divide os dois triângulos.
