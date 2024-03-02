#include "engine_pch.h"
#include "Model.h"

#include "stb_image.h"

#include "Renderer.h"
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

    std::vector<std::shared_ptr<Texture>> ProcessTextures(const aiScene* scene, const aiMaterial* mat, const aiTextureType& aiType, TextureUnit unit, TextureFormat imgFormat = RGB)
    {
        std::vector<std::shared_ptr<Texture>> retTextures;

        for (unsigned int i = 0; i < mat->GetTextureCount(aiType); i++)
        {
            aiString texturePath;
            //mat->GetTexture(aiType, i, &texturePath);
            mat->Get(AI_MATKEY_TEXTURE(aiType, i), texturePath);
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
                    type = TextureType::DIFFUSE;
                    Logger::Log(LogCategory::WARNING, "Unhandled texture type found in model", "Model::ProcessTextures");
                    break;
                }
                
                const aiTexture* textureEmb = scene->GetEmbeddedTexture(texturePath.C_Str());
                std::shared_ptr<Texture> texture;

                if (textureEmb != nullptr)
                {
                    int width;
                    int height;
                    int channels;
                    const uint8_t* texData = stbi_load_from_memory((stbi_uc*)textureEmb->pcData, textureEmb->mWidth, &width, &height, &channels, 0);
                    texture = std::make_shared<Texture>(type, texData, unit, RGB, imgFormat, UNSIGNED_BYTE, width, height);
                }
                else
                {
                    const std::string fullPath = directory + "/" + texturePath.C_Str();
                    texture = std::make_shared<Texture>(fullPath, type);
                }

                loadedTextures.push_back(texture);
                retTextures.push_back(texture);
            }
        }

        return retTextures;
    }

    void ProcessMesh(aiMesh* mesh, const aiScene* scene)
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

        std::vector<std::shared_ptr<Texture>> diffuseMaps  = ProcessTextures(scene, material, aiTextureType_BASE_COLOR, TEX_0);
        std::vector<std::shared_ptr<Texture>> normalMaps   = ProcessTextures(scene, material, aiTextureType_NORMALS, TEX_1);
        std::vector<std::shared_ptr<Texture>> roughMaps    = ProcessTextures(scene, material, aiTextureType_DIFFUSE_ROUGHNESS, TEX_2);
        std::vector<std::shared_ptr<Texture>> ambientMaps  = ProcessTextures(scene, material, aiTextureType_AMBIENT, TEX_3);
        std::vector<std::shared_ptr<Texture>> metallicMaps = ProcessTextures(scene, material, aiTextureType_METALNESS, TEX_4, BGR);
        std::vector<std::shared_ptr<Texture>> emissionMaps = ProcessTextures(scene, material, aiTextureType_EMISSIVE, TEX_5);

        // Only use the first available diffuse texture for now
        // FIX IN THE FUTURE
        const std::string vertPath = PathBuilder::GetPath("./Engine/shaders/vertexStandard.glsl");
        const std::string fragPath = PathBuilder::GetPath("./Engine/shaders/fragmentPBR.glsl");

        const std::shared_ptr<Shader> meshShader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
        const std::shared_ptr<Material> meshMat  = std::make_shared<Material>(meshShader);

        meshMat->SetTexture("irradianceMap", Renderer::SkyboxIrradianceMap);
        meshMat->SetTexture("prefilterMap", Renderer::SkyboxPrefilteredMap);
        meshMat->SetTexture("brdfMap", Renderer::SkyboxBRDFMap);
        meshMat->SetProperty("hasImageBasedLighting", true);
        
        if (!diffuseMaps.empty())
        {
            meshMat->SetTexture("albedoMap", diffuseMaps[0]);
            meshMat->SetProperty("hasMapAlbedo", true);
        }
        else
        {
            meshMat->SetProperty("albedo", glm::vec3(0.85f, 0.1f, 0.1f));
        }
        
        if (!normalMaps.empty())
        {
            meshMat->SetTexture("normalMap", normalMaps[0]);
            meshMat->SetProperty("hasMapNormal", true);
        }
        
        if (!metallicMaps.empty())
        {
            meshMat->SetTexture("metallicMap", metallicMaps[0]);
            meshMat->SetProperty("hasMapMetallic", true);
        }
        else
        {
            meshMat->SetProperty("metallic", 0.0f);
        }
        
        if (!roughMaps.empty())
        {
            meshMat->SetTexture("roughnessMap", roughMaps[0]);
            meshMat->SetProperty("hasMapRoughness", true);
        }
        else
        {
            meshMat->SetProperty("roughness", 0.3f);
        }
        
        if (!ambientMaps.empty())
        {
            meshMat->SetTexture("ambientOcclusionMap", ambientMaps[0]);
            meshMat->SetProperty("hasMapAmbientOcclusion", true);
        }
        else
        {
            meshMat->SetProperty("ambientOcclusion", 0.3f);
        }
        
        if (!emissionMaps.empty())
        {
            meshMat->SetTexture("emissionMap", emissionMaps[0]);
            meshMat->SetProperty("hasMapEmission", true);
        }

        std::shared_ptr<Mesh> ret = std::make_shared<Mesh>(vertices, indices, meshMat);

        if (!hasNormals)
        {
            Logger::Log(LogCategory::WARNING, "Imported mesh contains no normals", "Model::ProcessMesh");
        }

        if (!hasTangents)
        {
            Logger::Log(LogCategory::WARNING, "Imported mesh contains no tangents/bitangents. Attempting to generate...", "Model::ProcessMesh");
            ProceduralMesh::GenerateTangentsBitangents(ret);
        }
        
        loadedMeshes.push_back(ret);
    }

    void ProcessNode(const aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene);
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
    constexpr unsigned int importFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace;
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
