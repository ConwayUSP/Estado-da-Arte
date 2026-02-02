#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement { FRENTE, TRAS, ESQUERDA, DIREITA };

// Valores iniciais padrão
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float VELOCIDADE = 2.5f;
const float SENSITIVIDADE = 0.05f;
const float ZOOM = 45.0f;

class Camera {
public:
  glm::vec3 Posicao;
  glm::vec3 Direcao;
  glm::vec3 Cima;
  glm::vec3 Direita;
  glm::vec3 CimaMundo;
  float Yaw;
  float Pitch;
  float Velocidade;
  float Sensitividade;
  float Zoom;

  // Construtor com alguns valores padrões
  Camera(glm::vec3 posicao = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 cima = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH);

  // Função que forma a matriz view
  glm::mat4 MatrizView();
  // Callbacks de movimento e zoom
  void ProcessaTeclado(Camera_Movement direction, float deltaTime);
  void ProcessaMovimentoMouse(float offsetX, float offsetY,
                              GLboolean limitarPitch = true);
  void ProcessaScroll(float offsetY);

private:
  // Recalcula os vetores de direção, direita e cima de
  // acordo com os ângulos de Euler
  void atualizaEixos();
};
#endif
