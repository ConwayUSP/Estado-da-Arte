#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <glad/glad.h>

class Triangle {
public:
    Triangle();  // Compila shaders e configura buffers
    ~Triangle(); // Destrutor da classe: serve para liberar os recursos na GPU

    void draw(); // Ativa o shader e desenha

private:
    unsigned int VAO, VBO, shaderProgram;
    void setupShaders(); // Função auxiliar para compilar os shaders embutidos
};

#endif