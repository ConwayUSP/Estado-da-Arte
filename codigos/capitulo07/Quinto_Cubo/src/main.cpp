#include <glad/glad.h>

#include "modules/camera.hpp"
#include "modules/shader.hpp"
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>
#include <string>

void callback_redimensionamento(GLFWwindow *janela, int largura, int altura);
void processaInput(GLFWwindow *window);
std::string readTextFile(const std::string &file_path);
void callback_mouse(GLFWwindow *janela, double posX, double posY);
void callback_scroll(GLFWwindow *window, double offsetX, double offsetY);

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
bool primeiroInputMouse = true;
float ultimoX = 400, ultimoY = 300;

float dt = 0.0f;
float ultimoTempo = 0.0f;

int main() {

  float vertices[] = {
      // posições           // normais
      // Face Traseira
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
      -1.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f, 0.0f,
      0.0f, -1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f,
      0.0f, 0.0f, -1.0f,

      // Face Frontal
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

      // Face Esquerda
      -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, -1.0f, 0.0f,
      0.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, -1.0f,
      0.0f, 0.0f, -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f,
      -1.0f, 0.0f, 0.0f,

      // Face Direita
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
      0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
      0.0f,

      // Face Inferior
      -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 0.0f, -1.0f,
      0.0f, 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 0.0f,
      -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f,
      0.0f, -1.0f, 0.0f,

      // Face Superior
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
      0.0f};

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
    std::cout << "Falha ao inicializar o GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetFramebufferSizeCallback(window, callback_redimensionamento);
  glfwSetCursorPosCallback(window, callback_mouse);
  glfwSetScrollCallback(window, callback_scroll);

  //  Habilitar o teste de profundidade para o 3D funcionar
  glEnable(GL_DEPTH_TEST);

  /////////////////////////////////////////////////////////////////////

  Shader lightingShader("shaders/vertex.vert", "shaders/fragment.frag");

  Shader lightCubeShader(
      "shaders/vertex.vert",
      "shaders/lightCube.frag"); // Crie este frag que retorna vec4(1.0)

  // 1. Configuração ÚNICA de Memória (Fora do loop)
  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  // Prepara o VBO e o VAO do Cubo Principal
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Posições (location = 0)
  glBindVertexArray(cubeVAO);
  // Posições (stride de 6 floats)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Normais (location = 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Prepara o VAO do Cubo de Luz
  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Reaproveita o VBO já preenchido
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
  lightingShader.setVec3("lightPos", lightPos);

  // 2. LOOP PRINCIPAL DE RENDERIZAÇÃO
  while (!glfwWindowShouldClose(window)) {
    // -- Lógica de Tempo --
    float tempoAtual = glfwGetTime();
    dt = tempoAtual - ultimoTempo;
    ultimoTempo = tempoAtual;

    // -- Input --
    processaInput(window); // Agora você vai conseguir mover a câmera!

    // -- Limpeza da Tela --
    glClearColor(0.1f, 0.1f, 0.1f,
                 1.0f); // Um fundo cinza escuro pra dar contraste
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // -- Matrizes de Câmera (Atualizadas todo frame para movimento) --
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    glm::mat4 view = camera.MatrizView();

    // -- DESENHA O CUBO PRINCIPAL --
    lightingShader.use();
    lightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setVec3("lightPos", lightPos);
    lightingShader.setVec3("viewPos", camera.Posicao);
    lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // -- DESENHA O CUBO DE LUZ --
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);

    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // Deixa a luz menor
    lightCubeShader.setMat4("model", model);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // -- Troca os buffers --
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // 3. Limpeza de Memória
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightCubeVAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}

void callback_redimensionamento(GLFWwindow *janela, int largura, int altura) {
  glViewport(0, 0, largura, altura);
}

void processaInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessaTeclado(FRENTE, dt);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessaTeclado(TRAS, dt);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessaTeclado(ESQUERDA, dt);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessaTeclado(DIREITA, dt);
}

std::string readTextFile(const std::string &file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << file_path << std::endl;
    return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  file.close();

  return buffer.str();
}

void callback_mouse(GLFWwindow *janela, double posXin, double posYin) {
  float posX = static_cast<float>(posXin);
  float posY = static_cast<float>(posYin);

  if (primeiroInputMouse) {
    ultimoX = posX;
    ultimoY = posY;
    primeiroInputMouse = false;
  }

  float offsetX = posX - ultimoX;
  float offsetY = ultimoY - posY;

  ultimoX = posX;
  ultimoY = posY;

  camera.ProcessaMovimentoMouse(offsetX, offsetY);
}

void callback_scroll(GLFWwindow *window, double offsetX, double offsetY) {
  camera.ProcessaScroll(static_cast<float>(offsetY));
}
