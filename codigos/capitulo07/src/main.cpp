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


int main(){


    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f);

    return 0;
}
