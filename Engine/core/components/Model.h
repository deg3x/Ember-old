#pragma once

#include "Component.h"

#include <vector>
#include <memory>

class Mesh;
struct aiNode;
struct aiScene;
struct aiMesh;

class Model : public Component
{
private:
    std::vector<std::shared_ptr<Mesh>> meshes;
    
public:
    Model() = default;
    Model(const char* path);
    virtual ~Model() = default;

    void Draw() const;

private:
    void Load(const char* path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    //std::vector<Texture> ProcessTextures();
};