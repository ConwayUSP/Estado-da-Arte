#ifndef MESH_H
#define MESH_H
#include "shader.hpp"
#include <glm/glm.hpp>
#include <string>
#include <vector>
using namespace std;

struct Vertice {
  glm::vec3 Posicoes;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Textura {
  unsigned int id;
  string tipo;
};

class Mesh {
public:
  vector<Vertice> vertices;
  vector<unsigned int> indices;
  vector<Textura> texturas;

  Mesh(vector<Vertice> vertices, vector<unsigned int> indices,
       vector<Textura> texturas);
  void Draw(Shader &shader);

private:
  unsigned int VAO, VBO, EBO;

  void setupMesh();
};

#endif
