#include "triangle.hpp"
#include <iostream>

// Shaders embutidos (Hardcoded, no próximo capítulos abordaremos de maneira mais elegante)
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(0.5f, 0.0f, 0.5f, 1.0f);\n" // Roxo DASI
    "}\n\0";

Triangle::Triangle() {
    // Setup dos Shaders (Magia que será explicada no próximo capítulo)
    setupShaders();

    // Vértices do triângulo
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Esquerda 
         0.5f, -0.5f, 0.0f, // Direita
         0.0f,  0.5f, 0.0f  // Topo
    };

    // VBO: O balde de dados
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // VAO: O manual de instruções
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Ensinando a GPU 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Novamente, essa parte não é o foco do capítulo 01, então não se preocupe em entender cada linha
void Triangle::setupShaders() {
    // Criamos o Vertex Shader 
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 
    glCompileShader(vertexShader); 

    // Criamos o Fragment Shader 
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);   
    glAttachShader(shaderProgram, fragmentShader); 
    glLinkProgram(shaderProgram);                  

    // Limpeza
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Triangle::~Triangle() {
    // Quando o objeto Triangle for destruído no C++, 
    // precisamos dizer para a GPU liberar essas "gavetas" (IDs)
    glDeleteVertexArrays(1, &VAO);   // Deleta o manual de instruções
    glDeleteBuffers(1, &VBO);        // Esvazia o balde de dados
    glDeleteProgram(shaderProgram);  // Apaga o programa de shader da memória
}

void Triangle::draw() {
    // Dizemos ao OpenGL: "Use este conjunto de shaders"
    glUseProgram(shaderProgram);

    // Dizemos: "Siga as instruções que salvamos neste manual (VAO)"
    // Isso já diz automaticamente qual VBO usar e como ler os dados
    glBindVertexArray(VAO);

    // Ordem de execução: Desenhe triângulos, comece no índice 0 e use 3 vértices
    glDrawArrays(GL_TRIANGLES, 0, 3);
}