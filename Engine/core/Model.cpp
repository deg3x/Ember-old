#include "engine_pch.h"
#include "Model.h"

#include "core/components/Mesh.h"
#include "core/textures/TextureDiffuse.h"
#include "core/materials/MaterialBlinnPhong.h"
#include "utils/PathBuilder.h"
#include "logger/Logger.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace
{
    std::vector<std::shared_ptr<Mesh>> loadedMeshes;
    std::vector<std::shared_ptr<TextureDiffuse>> loadedTextures;

    std::string directory;

    std::vector<std::shared_ptr<TextureDiffuse>> ProcessTextures(const aiMaterial* mat, const aiTextureType& aiType)
    {
        std::vector<std::shared_ptr<TextureDiffuse>> retTextures;

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
                std::shared_ptr<TextureDiffuse> texture = std::make_shared<TextureDiffuse>(fullPath.c_str());
                loadedTextures.push_back(texture);
                retTextures.push_back(texture);
            }
        }

        return retTextures;
    }

    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene)
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

        std::vector<std::shared_ptr<TextureDiffuse>> diffuseMaps  = ProcessTextures(material, aiTextureType_DIFFUSE);
        std::vector<std::shared_ptr<TextureDiffuse>> normalMaps   = ProcessTextures(material, aiTextureType_NORMALS);
        std::vector<std::shared_ptr<TextureDiffuse>> specularMaps = ProcessTextures(material, aiTextureType_SPECULAR);
        std::vector<std::shared_ptr<TextureDiffuse>> heightMaps   = ProcessTextures(material, aiTextureType_HEIGHT);

        std::vector<std::shared_ptr<TextureDiffuse>> meshTextures;
        meshTextures.insert(meshTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
        meshTextures.insert(meshTextures.end(), normalMaps.begin(), normalMaps.end());
        meshTextures.insert(meshTextures.end(), specularMaps.begin(), specularMaps.end());
        meshTextures.insert(meshTextures.end(), heightMaps.begin(), heightMaps.end());

        // Only use the first available diffuse texture for now
        // FIX IN THE FUTURE
        std::shared_ptr<MaterialBlinnPhong> meshMaterial = std::make_shared<MaterialBlinnPhong>();
        if (!diffuseMaps.empty())
            meshMaterial->SetDiffuseTexture(diffuseMaps[0]);
        
        return new Mesh(vertices, indices, meshMaterial);
    }

    void ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            loadedMeshes.push_back(std::shared_ptr<Mesh>(ProcessMesh(mesh, scene)));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
}

std::vector<std::shared_ptr<Mesh>> Model::Load(const char* path)
{
    std::vector<std::shared_ptr<Mesh>>().swap(loadedMeshes);
    
    Assimp::Importer importer;
    constexpr unsigned int importFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
    const aiScene* scene = importer.ReadFile(PathBuilder::GetPath(path), importFlags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Logger::Log(LogCategory::ERROR, importer.GetErrorString(), "Model::Load");
        return loadedMeshes;
    }

    directory = path;
    directory = directory.substr(0, directory.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
    return loadedMeshes;
}