#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <string>

#include "modules/triangle.hpp"

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

    GLFWwindow *window = glfwCreateWindow(800, 600, "Trilha OpenGL - Triangulo Roxo", NULL, NULL);
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

    // Instanciamos nosso objeto modular
    // O construtor já cuida de enviar os dados para a GPU
    Triangle meuTriangulo;

    while (!glfwWindowShouldClose(window)) {
        processaInput(window);
        glClearColor(0.1f, 0.64f, 0.64f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Desenha o triângulo
        meuTriangulo.draw();

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