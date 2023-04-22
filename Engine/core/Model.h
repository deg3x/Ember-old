#pragma once

#include "components/Component.h"

#include <vector>
#include <memory>

class Mesh;
struct aiNode;
struct aiScene;
struct aiMesh;

class Model
{
public:
    Model() = delete;
    
    static std::vector<std::shared_ptr<Mesh>> Load(const char* path);

private:
    static void ProcessNode(aiNode* node, const aiScene* scene);
    static Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    //static std::vector<Texture> ProcessTextures();
};