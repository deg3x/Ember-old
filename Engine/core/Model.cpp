#include "engine_pch.h"
#include "Model.h"

#include "Shader.h"
#include "core/components/Mesh.h"
#include "core/Material.h"
#include "core/Texture.h"
#include "utils/PathBuilder.h"
#include "logger/Logger.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "utils/ProceduralMesh.h"

namespace
{
    std::vector<std::shared_ptr<Mesh>> loadedMeshes;
    std::vector<std::shared_ptr<Texture>> loadedTextures;

    std::string directory;

    std::vector<std::shared_ptr<Texture>> ProcessTextures(const aiMaterial* mat, const aiTextureType& aiType)
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
                // FILL IN MORE TEXTURE TYPES IN THE FUTURE
                TextureType type = TextureType::DIFFUSE;
                switch(aiType)
                {
                case aiTextureType_NONE:
                    break;
                case aiTextureType_DIFFUSE:
                    type = TextureType::DIFFUSE;
                    break;
                default:
                    Logger::Log(LogCategory::WARNING, "Unhandled texture type found in model", "Model::ProcessTextures");
                    break;
                }
                
                std::string fullPath = directory + "/" + texturePath.C_Str();
                std::shared_ptr<Texture> texture = std::make_shared<Texture>(fullPath, type);

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

        const bool hasNormals  = mesh->HasNormals();
        const bool hasTangents = mesh->HasTangentsAndBitangents();

        // Vertex Processing
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            VertexData vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            if (hasNormals)
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }

            if (hasTangents)
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

        std::vector<std::shared_ptr<Texture>> diffuseMaps  = ProcessTextures(material, aiTextureType_DIFFUSE);
        std::vector<std::shared_ptr<Texture>> normalMaps   = ProcessTextures(material, aiTextureType_NORMALS);
        std::vector<std::shared_ptr<Texture>> specularMaps = ProcessTextures(material, aiTextureType_SPECULAR);
        std::vector<std::shared_ptr<Texture>> heightMaps   = ProcessTextures(material, aiTextureType_HEIGHT);

        std::vector<std::shared_ptr<Texture>> meshTextures;
        meshTextures.insert(meshTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
        meshTextures.insert(meshTextures.end(), normalMaps.begin(), normalMaps.end());
        meshTextures.insert(meshTextures.end(), specularMaps.begin(), specularMaps.end());
        meshTextures.insert(meshTextures.end(), heightMaps.begin(), heightMaps.end());

        // Only use the first available diffuse texture for now
        // FIX IN THE FUTURE
        const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexStandard.glsl");
        const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentPBR.glsl");

        const std::shared_ptr<Shader> meshShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
        const std::shared_ptr<Material> meshMat  = std::make_shared<Material>(meshShader);

        meshMat->SetProperty("albedo", glm::vec3(0.85f, 0.1f, 0.1f));
        meshMat->SetProperty("roughness", 0.3f);
        meshMat->SetProperty("metallic", 0.0f);
        meshMat->SetProperty("ambientOcclusion", 0.3f);
        
        if (!diffuseMaps.empty())
        {
            meshMat->SetTexture("diffuseTexture", diffuseMaps[0]);
        }
        else
        {
            std::shared_ptr<Texture> tex = std::make_shared<Texture>("./Data/images/white.png", TextureType::DIFFUSE);
            meshMat->SetTexture("diffuseTexture", tex);
        }

        Mesh* ret = new Mesh(vertices, indices, meshMat);

        if (!hasNormals)
        {
            Logger::Log(LogCategory::WARNING, "Imported mesh contains no normals", "Model::ProcessMesh");
        }

        if (!hasTangents)
        {
            Logger::Log(LogCategory::WARNING, "Imported mesh contains no tangents/bitangents.", "Model::ProcessMesh");
        }
        
        return ret;
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