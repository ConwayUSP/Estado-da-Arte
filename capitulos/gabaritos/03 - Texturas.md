# Exercícios Propostos

## Exercício 1

1. Defina com `false`, comente ou apague a linha a seguir no `main.cpp`:

```cpp
stbi_set_flip_vertically_on_load(true);
```

2. Invertida verticalmente

- Troca os valores de T (coordenada vertical): o que era `1.0` vira `0.0` e vice-versa.

```cpp
float vertices[] = {
    // posicoes          // coords de textura
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f,   // superior direito
     0.5f, -0.5f, 0.0f,  1.0f, 1.0f,   // inferior direito
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f,   // inferior esquerdo
    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f    // superior esquerdo
};
```

3. Espelhada horizontalmente

- Troca os valores de S (coordenada horizontal): o que era `1.0` vira `0.0` e vice-versa.

```cpp
float vertices[] = {
    // posicoes          // coords de textura
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f,   // superior direito
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // inferior direito
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // inferior esquerdo
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f    // superior esquerdo
};
```

A origem do sistema UV fica no canto inferior esquerdo da textura: (0,0) é o canto inferior esquerdo e (1,1) é o superior direito. Isso significa que:

- Inverter T faz com que os vértices do topo da tela recebam T=0 e os de baixo recebam T=1, produzindo o flip vertical.
- Inverter S faz com que os vértices da direita recebam S=0,e os vértices da esquerda recebam S=1, produzindo o espelhamento horizontal.

A inversão em T é exatamente o problema que `stbi_set_flip_vertically_on_load(true)` resolve automaticamente no eixo vertical: sem ele, a imagem já apareceria invertida, pois os formatos de arquivo de imagem (.jpg, .png) armazenam os pixels de cima para baixo, enquanto o OpenGL os espera de baixo para cima.

## Exercício 2

Exemplo de solução:

1. No array de vértices, substitua os 1.0f das coordenadas UV por 2.0f:

```cpp
float vertices[] = {
    // posicoes          // coords de textura
     0.5f,  0.5f, 0.0f,  2.0f, 2.0f,   // superior direito
     0.5f, -0.5f, 0.0f,  2.0f, 0.0f,   // inferior direito
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // inferior esquerdo
    -0.5f,  0.5f, 0.0f,  0.0f, 2.0f    // superior esquerdo
};
```

2. Configuração dos quatro modos:

Substitua os `glTexParameteri` de wrapping da sua textura pelo modo desejado e veja o efeito:

```cpp
// GL_REPEAT (padrão — textura repete em grade 2×2):
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

// GL_MIRRORED_REPEAT (repete espelhando a cada tile):
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

// GL_CLAMP_TO_EDGE (borda da imagem se estica para preencher o espaço extra):
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

// GL_CLAMP_TO_BORDER (área fora de [0,1] recebe uma cor sólida — aqui, laranja):
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

float corDaBorda[] = { 1.0f, 0.5f, 0.0f, 1.0f }; // laranja
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, corDaBorda);
```
