#include "shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char *verticePath, const char *fragmentoPath) {
  // 1 - Pegamos os códigos dos shaders e colocamos eles em variáveis
  std::string codigoVS;
  std::string codigoFS;
  std::ifstream arquivoVS;
  std::ifstream arquivoFS;
  arquivoVS.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  arquivoFS.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    arquivoVS.open(verticePath);
    arquivoFS.open(fragmentoPath);
    std::stringstream streamVS, streamFS;
    // Lendo o conteúdo dos arquivos em streams
    streamVS << arquivoVS.rdbuf();
    streamFS << arquivoFS.rdbuf();

    arquivoVS.close();
    arquivoFS.close();
    // Convertendo as streams em strings
    codigoVS = streamVS.str();
    codigoFS = streamFS.str();
  } catch (const std::ifstream::failure &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
  const char *codigoVSbruto = codigoVS.c_str();
  const char *codigoFSbruto = codigoFS.c_str();
  unsigned int vertice, fragmento;
  int sucesso;
  char infoLog[512];

  // Shader de vértice
  vertice = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertice, 1, &codigoVSbruto, NULL);
  glCompileShader(vertice);
  // Tratando um possível erro
  glGetShaderiv(vertice, GL_COMPILE_STATUS, &sucesso);
  if (!sucesso) {
    glGetShaderInfoLog(vertice, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  };

  // Repetindo para o shader de fragmento
  fragmento = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmento, 1, &codigoFSbruto, NULL);
  glCompileShader(fragmento);
  glGetShaderiv(fragmento, GL_COMPILE_STATUS, &sucesso);
  if (!sucesso) {
    glGetShaderInfoLog(fragmento, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  };

  // Criando o programa
  ID = glCreateProgram(); // ID é propriedade da própria classe Shader
  glAttachShader(ID, vertice);
  glAttachShader(ID, fragmento);
  glLinkProgram(ID);
  // Checando erros de linkagem
  glGetProgramiv(ID, GL_LINK_STATUS, &sucesso);
  if (!sucesso) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  // Limpando os shaders criados
  glDeleteShader(vertice);
  glDeleteShader(fragmento);
}

void Shader::use() { glUseProgram(ID); }

void Shader::setBool(const std::string &nome, bool valor) const {
  glUniform1i(glGetUniformLocation(ID, nome.c_str()), (int)valor);
}
void Shader::setInt(const std::string &nome, int valor) const {
  glUniform1i(glGetUniformLocation(ID, nome.c_str()), valor);
}
void Shader::setFloat(const std::string &nome, float valor) const {
  glUniform1f(glGetUniformLocation(ID, nome.c_str()), valor);
}
