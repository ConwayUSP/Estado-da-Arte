#include <glad/glad.h>

#include "modules/camera.hpp"
#include "modules/model.hpp"
#include "modules/shader.hpp"
#include "modules/stb_image.h"
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

glm::vec3 luzPos(2.0f, 1.0f, 4.0f);

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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetFramebufferSizeCallback(window, callback_redimensionamento);
  glfwSetCursorPosCallback(window, callback_mouse);
  glfwSetScrollCallback(window, callback_scroll);
  glEnable(GL_DEPTH_TEST);

  Shader meuShaderInsano("shaders/vertex.vert", "shaders/fragment.frag");

  Modelo shrekWazowski(string("models/shrek_wazowski/scene.gltf"));

  while (!glfwWindowShouldClose(window)) {
    float tempoAtual = glfwGetTime();
    dt = tempoAtual - ultimoTempo;
    ultimoTempo = tempoAtual;
    processaInput(window);

    glClearColor(0.1f, 0.64f, 0.64f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    meuShaderInsano.use();
    meuShaderInsano.setVec3("visaoPos", camera.Posicao);
    meuShaderInsano.setVec3("luz.posicao", luzPos);
    meuShaderInsano.setVec3("luz.ambiente", glm::vec3(0.2f, 0.3f, 0.3f));
    meuShaderInsano.setVec3("luz.difuso", glm::vec3(0.5f, 0.5f, 0.5f));
    meuShaderInsano.setVec3("luz.especular", glm::vec3(1.0f, 1.0f, 1.0f));

    glm::mat4 model = glm::mat4(1.0);
    model = glm::scale(model, glm::vec3(0.05));
    model = glm::translate(model, glm::vec3(0.0, -20.0, 0.0));
    glm::mat4 view;
    view = camera.MatrizView();
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f,
                                  0.1f, 100.0f);

    meuShaderInsano.setMat4("model", model);
    meuShaderInsano.setMat4("view", view);
    meuShaderInsano.setMat4("projection", projection);

    shrekWazowski.Draw(meuShaderInsano);

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
