#include "mesh.hpp"

Mesh::Mesh(vector<Vertice> vertices, vector<unsigned int> indices,
           vector<Textura> texturas) {
  this->vertices = vertices;
  this->indices = indices;
  this->texturas = texturas;

  setupMesh();
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertice), &vertices[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // posições dos vértices
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void *)0);
  // normais dos vértices
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice),
                        (void *)offsetof(Vertice, Normal));
  // coordenadas de textura dos vértices
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertice),
                        (void *)offsetof(Vertice, TexCoords));

  glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) {
  unsigned int nDifusa = 1;
  unsigned int nAO = 1;
  unsigned int nRugosa = 1;
  unsigned int nMetal = 1;
  for (unsigned int i = 0; i < texturas.size(); i++) {
    // Ativando uma unidade de textura nova
    glActiveTexture(GL_TEXTURE0 + i);
    string numero;
    string nome = texturas[i].tipo;
    if (nome == "textura_difusa")
      numero = std::to_string(nDifusa++);
    else if (nome == "textura_AO")
      numero = std::to_string(nAO++);
    else if (nome == "textura_rugosa")
      numero = std::to_string(nRugosa++);
    else if (nome == "textura_metal")
      numero = std::to_string(nMetal++);

    shader.setInt((nome + numero).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, texturas[i].id);
  }
  glActiveTexture(GL_TEXTURE0);

  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
