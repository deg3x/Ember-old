#include "TextureCubemap.h"

#include "glad/glad.h"
#include "stb_image.h"

#include "utils/PathBuilder.h"
#include "logger/Logger.h"

TextureCubemap::TextureCubemap()
{
    path = "./Data/images/skybox/cubemap_clouds_";

    TextureCubemap::InitializeTexture(PathBuilder::GetPath("./Data/images/skybox/cubemap_clouds_").c_str());
}

TextureCubemap::TextureCubemap(const char* texturePath)
{
    path = texturePath;

    TextureCubemap::InitializeTexture(PathBuilder::GetPath(texturePath).c_str());
}

void TextureCubemap::Use() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void TextureCubemap::InitializeTexture(const char* texturePath)
{
    int width, height, nChannels;

    const std::string faces[6] = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false);

    for (unsigned int i = 0; i < 6; i++)
    {
        std::string fullPath = texturePath;
        fullPath += faces[i];
            
        unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            Logger::Log(LogCategory::ERROR, "Cubemap tex failed to load at path: " + fullPath, "TextureCubemap::InitializeTexture");
        }
        
        stbi_image_free(data);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
