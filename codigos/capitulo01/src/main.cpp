#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void callback_redimensionamento(GLFWwindow *janela, int largura, int altura);
void processaInput(GLFWwindow *window);

// Shaders embutidos (Abordaremos de maneira elegante no próximo capítulo)
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(0.5f, 0.0f, 0.5f, 1.0f);\n" // Roxo 
    "}\n\0";

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

    // Setup dos shaders ( É uma magia temporária)
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // VBO & VAO
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Esquerda 
         0.5f, -0.5f, 0.0f, // Direita
         0.0f,  0.5f, 0.0f  // Topo
    };

    unsigned int VBO, VAO;

    // Geramos e vinculamos o VAO primeiro para "gravar" as configurações que virão abaixo
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Configuramos o VBO (o balde de dados)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Ensinamos a GPU como ler os dados
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Ativa o atributo no índice 0 (que corresponde ao "layout (location = 0)" do vertex shader)
    glEnableVertexAttribArray(0);

    // Desvinculamos para evitar bagunça (opcional, mas boa prática)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Loop de renderização
    while (!glfwWindowShouldClose(window)) {
        processaInput(window);
        glClearColor(0.1f, 0.64f, 0.64f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Ativamos o shader e o manual de instruções (VAO)
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);       // Resgata todo o estado configurado
        
        // O triângulo finalmente aparece!
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Libera a memória alocada na GPU antes de fechar o programa
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

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