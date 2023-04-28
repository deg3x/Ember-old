#include "Model.h"

#include "Texture.h"
#include "Material.h"
#include "components/meshes/Mesh.h"
#include "../utils/PathBuilder.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <iostream>

namespace
{
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Texture>> loadedTextures;

    std::string directory;
}

std::vector<std::shared_ptr<Texture>> ProcessTextures(aiMaterial* mat, aiTextureType aiType, TextureType type);

std::vector<std::shared_ptr<Mesh>> Model::Load(const char* path)
{
    std::vector<std::shared_ptr<Mesh>>().swap(meshes);
    
    Assimp::Importer importer;
    constexpr unsigned int importFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
    const aiScene* scene = importer.ReadFile(PathBuilder::GetPath(path), importFlags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "[Assimp] Error: " << importer.GetErrorString() << std::endl;
        return meshes;
    }

    directory = path;
    directory = directory.substr(0, directory.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
    return meshes;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(std::shared_ptr<Mesh>(ProcessMesh(mesh, scene)));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indices;

    // Vertex Processing
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        VertexData vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        if (mesh->HasTangentsAndBitangents())
        {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;
            
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 v;

            v.x = mesh->mTextureCoords[0][i].x;
            v.y = mesh->mTextureCoords[0][i].y;
            vertex.uv = v;
        }
        else
        {
            vertex.uv = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Face Processing
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<std::shared_ptr<Texture>> diffuseMaps  = ProcessTextures(material, aiTextureType_DIFFUSE, TextureType::diffuse);
    std::vector<std::shared_ptr<Texture>> normalMaps   = ProcessTextures(material, aiTextureType_NORMALS, TextureType::normal);
    std::vector<std::shared_ptr<Texture>> specularMaps = ProcessTextures(material, aiTextureType_SPECULAR, TextureType::diffuse);
    std::vector<std::shared_ptr<Texture>> heightMaps   = ProcessTextures(material, aiTextureType_HEIGHT, TextureType::diffuse);

    std::vector<std::shared_ptr<Texture>> meshTextures;
    meshTextures.insert(meshTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
    meshTextures.insert(meshTextures.end(), normalMaps.begin(), normalMaps.end());
    meshTextures.insert(meshTextures.end(), specularMaps.begin(), specularMaps.end());
    meshTextures.insert(meshTextures.end(), heightMaps.begin(), heightMaps.end());
    
    std::shared_ptr<Material> meshMaterial = std::make_shared<Material>();
    meshMaterial->SetTextures(meshTextures);
    
    return new Mesh(vertices, indices, meshMaterial);
}

std::vector<std::shared_ptr<Texture>> ProcessTextures(aiMaterial* mat, aiTextureType aiType, TextureType type)
{
    std::vector<std::shared_ptr<Texture>> retTextures;

    for (unsigned int i = 0; i < mat->GetTextureCount(aiType); i++)
    {
        aiString texturePath;
        mat->GetTexture(aiType, i, &texturePath);
        bool alreadyLoaded = false;

        for (unsigned int j = 0; j < loadedTextures.size(); j++)
        {
            if (std::strcmp(loadedTextures[j]->GetPath().c_str(), texturePath.C_Str()) == 0)
            {
                retTextures.push_back(loadedTextures[j]);
                alreadyLoaded = true;
                break;
            }
        }

        if (!alreadyLoaded)
        {
            std::string fullPath = directory + "/" + texturePath.C_Str();
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(type, fullPath.c_str());
            loadedTextures.push_back(texture);
            retTextures.push_back(texture);
        }
    }

    return retTextures;
}