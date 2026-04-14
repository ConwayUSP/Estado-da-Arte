#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Shader {
public:
  // ID do programa associado os shaders
  unsigned int ID;

  // O construtor cuida da parte repetitiva de ler arquivo,
  // compilar shaders, linkar, tratar erros, etc
  Shader(const char *verticePath, const char *fragmentoPath);
  // Ativa o shader com o ID ali de cima
  void use();
  // Facilitam a alteração de um valor uniform
  void setBool(const std::string &nome, bool valor) const;
  void setInt(const std::string &nome, int valor) const;
  void setFloat(const std::string &nome, float valor) const;
  void setVec3(const std::string &name, glm::vec3 vec) const;
  void setMat4(const std::string &name, glm::mat4 mat);
};

#endif
