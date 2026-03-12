#include <glad/glad.h>
#include "modules/shader.hpp"
#include "modules/stb_image.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void callback_redimensionamento(GLFWwindow *janela, int largura, int altura);
void processaInput(GLFWwindow *window);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "Estado da Arte", NULL, NULL);
    if (!window) {
        std::cout << "Erro ao criar janela do GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Falha ao carregar funções do GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, callback_redimensionamento);

    Shader meuShaderInsano("shaders/vertex.vert", "shaders/fragment.frag");

    // Agora cada vértice tem: posição (3 floats) + coord de textura (2 floats)
    float vertices[] = {
        // posicoes          // coords de textura
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // superior direito
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // inferior direito
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // inferior esquerdo
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f   // superior esquerdo
    };

    unsigned int indices[] = {
    0, 1, 3, // primeiro triângulo
    1, 2, 3  // segundo triângulo
};

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Posição (location = 0): stride agora é 5 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Coord de textura (location = 1): offset de 3 floats
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // --- Textura 1 ---
    unsigned int textura1;
    glGenTextures(1, &textura1);
    glBindTexture(GL_TEXTURE_2D, textura1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int largura, altura, nCanais;
    unsigned char *dados = stbi_load("textures/container.jpg", &largura, &altura, &nCanais, 0);
    if (dados) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, largura, altura, 0, GL_RGB, GL_UNSIGNED_BYTE, dados);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Falha ao carregar textura1!" << std::endl;
    }
    stbi_image_free(dados);

    // --- Textura 2 ---
    unsigned int textura2;
    glGenTextures(1, &textura2);
    glBindTexture(GL_TEXTURE_2D, textura2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Imagens PNG podem ter canal alpha (RGBA), então usamos GL_RGBA
    dados = stbi_load("textures/awesomeface.png", &largura, &altura, &nCanais, 0);
    if (dados) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, dados);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Falha ao carregar textura2!" << std::endl;
    }
    stbi_image_free(dados);

    // Conecta cada sampler do shader ao seu slot de textura
    meuShaderInsano.use();
    meuShaderInsano.setInt("textura1", 0);
    meuShaderInsano.setInt("textura2", 1);


    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(),glm::vec3(0.0f, 0.0f, 1.0f));

    unsigned int transformLoc = glGetUniformLocation(meuShaderInsano.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    while (!glfwWindowShouldClose(window)) {
        processaInput(window);

        glClearColor(0.1f, 0.64f, 0.64f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Vincula as texturas aos seus slots antes de desenhar
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textura1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textura2);

        meuShaderInsano.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void callback_redimensionamento(GLFWwindow *janela, int largura, int altura) {
    glViewport(0, 0, largura, altura);
}

void processaInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
