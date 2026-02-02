#ifndef MODEL_H
#define MODEL_H

#include "mesh.hpp"
#include "shader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
using namespace std;

class Modelo {
public:
  Modelo(string path);
  void Draw(Shader &shader);

private:
  vector<Mesh> meshes;
  string diretorio;

  void processaNo(aiNode *no, const aiScene *cena);
  Mesh processaMesh(aiMesh *mesh, const aiScene *cena);
  vector<Textura> carregaTexturas(aiMaterial *mat, aiTextureType tipo,
                                  string nomeTipo);
};

#endif
