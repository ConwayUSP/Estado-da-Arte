#include "model.hpp"
#include "stb_image.h"
#include <iostream>

unsigned int loadTexture(string path);

void Modelo::Draw(Shader &shader) {
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].Draw(shader);
}

Modelo::Modelo(string path) {
  Assimp::Importer importador;
  const aiScene *cena =
      importador.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!cena || cena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !cena->mRootNode) {
    cout << "ERROR::ASSIMP::" << importador.GetErrorString() << endl;
    return;
  }
  diretorio = path.substr(0, path.find_last_of('/'));

  processaNo(cena->mRootNode, cena);
}

void Modelo::processaNo(aiNode *no, const aiScene *cena) {
  // processando cada mesh do nó atual
  for (unsigned int i = 0; i < no->mNumMeshes; i++) {
    aiMesh *mesh = cena->mMeshes[no->mMeshes[i]];
    meshes.push_back(processaMesh(mesh, cena));
  }
  // chamadas recursivas para os nós filhos
  for (unsigned int i = 0; i < no->mNumChildren; i++) {
    processaNo(no->mChildren[i], cena);
  }
}

Mesh Modelo::processaMesh(aiMesh *mesh, const aiScene *cena) {
  vector<Vertice> vertices;
  vector<unsigned int> indices;
  vector<Textura> texturas;

  // Pega os atributos de vértice
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertice vertice;
    glm::vec3 vec;
    vec.x = mesh->mVertices[i].x;
    vec.y = mesh->mVertices[i].y;
    vec.z = mesh->mVertices[i].z;
    vertice.Posicoes = vec;
    vec.x = mesh->mNormals[i].x;
    vec.y = mesh->mNormals[i].y;
    vec.z = mesh->mNormals[i].z;
    vertice.Normal = vec;
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertice.TexCoords = vec;
    } else
      vertice.TexCoords = glm::vec2(0.0f, 0.0f);
    vertices.push_back(vertice);
  }
  // Pega os índices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  // Carrega texturas
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = cena->mMaterials[mesh->mMaterialIndex];
    vector<Textura> mapasDifusos =
        carregaTexturas(material, aiTextureType_DIFFUSE, "textura_difusa");
    texturas.insert(texturas.end(), mapasDifusos.begin(), mapasDifusos.end());
    vector<Textura> mapasAO = carregaTexturas(
        material, aiTextureType_AMBIENT_OCCLUSION, "textura_AO");
    texturas.insert(texturas.end(), mapasAO.begin(), mapasAO.end());
    vector<Textura> mapasRugosos = carregaTexturas(
        material, aiTextureType_DIFFUSE_ROUGHNESS, "textura_rugosa");
    texturas.insert(texturas.end(), mapasRugosos.begin(), mapasRugosos.end());
    vector<Textura> mapasMetalicos =
        carregaTexturas(material, aiTextureType_METALNESS, "textura_metal");
    texturas.insert(texturas.end(), mapasMetalicos.begin(),
                    mapasMetalicos.end());
  }

  return Mesh(vertices, indices, texturas);
}

vector<Textura> Modelo::carregaTexturas(aiMaterial *mat, aiTextureType tipo,
                                        string nomeTipo) {
  vector<Textura> texturas;
  for (unsigned int i = 0; i < mat->GetTextureCount(tipo); i++) {
    aiString str;
    mat->GetTexture(tipo, i, &str);
    Textura textura;
    textura.id = loadTexture(diretorio + '/' + string(str.C_Str()));
    textura.tipo = nomeTipo;
    texturas.push_back(textura);
  }
  return texturas;
}

unsigned int loadTexture(string path) {
  unsigned int IDtexura;
  glGenTextures(1, &IDtexura);

  int largura, altura, nrComponentes;
  unsigned char *dados =
      stbi_load(path.c_str(), &largura, &altura, &nrComponentes, 0);
  if (dados) {
    GLenum formato;
    if (nrComponentes == 1)
      formato = GL_RED;
    else if (nrComponentes == 3)
      formato = GL_RGB;
    else if (nrComponentes == 4)
      formato = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, IDtexura);
    glTexImage2D(GL_TEXTURE_2D, 0, formato, largura, altura, 0, formato,
                 GL_UNSIGNED_BYTE, dados);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(dados);
  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(dados);
  }

  return IDtexura;
}
