#include "camera.hpp"

Camera::Camera(glm::vec3 posicao, glm::vec3 cima, float yaw, float pitch)
    : Direcao(glm::vec3(0.0f, 0.0f, -1.0f)), Velocidade(VELOCIDADE),
      Sensitividade(SENSITIVIDADE), Zoom(ZOOM) {
  Posicao = posicao;
  CimaMundo = cima;
  Yaw = yaw;
  Pitch = pitch;
  atualizaEixos();
}

glm::mat4 Camera::MatrizView() {
  return glm::lookAt(Posicao, Posicao + Direcao, Cima);
}

void Camera::ProcessaTeclado(Camera_Movement direcao, float deltaTime) {
  float vel = Velocidade * deltaTime;
  if (direcao == FRENTE)
    Posicao += Direcao * vel;
  if (direcao == TRAS)
    Posicao -= Direcao * vel;
  if (direcao == ESQUERDA)
    Posicao -= Direita * vel;
  if (direcao == DIREITA)
    Posicao += Direita * vel;
}

void Camera::ProcessaMovimentoMouse(float offsetX, float offsetY,
                                    GLboolean limitarPitch) {
  offsetX *= Sensitividade;
  offsetY *= Sensitividade;

  Yaw += offsetX;
  Pitch += offsetY;

  if (limitarPitch) {
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;
  }

  atualizaEixos();
}

void Camera::ProcessaScroll(float offsetY) {
  Zoom -= (float)offsetY;
  if (Zoom < 1.0f)
    Zoom = 1.0f;
  if (Zoom > 60.0f)
    Zoom = 60.0f;
}

void Camera::atualizaEixos() {
  glm::vec3 direcao;
  direcao.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  direcao.y = sin(glm::radians(Pitch));
  direcao.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Direcao = glm::normalize(direcao);
  Direita = glm::normalize(glm::cross(direcao, CimaMundo));
  Cima = glm::normalize(glm::cross(Direita, direcao));
}
