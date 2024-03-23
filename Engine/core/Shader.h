#pragma once

#include "Definitions.h"

class ENGINE_API Shader
{
private:
    unsigned int programID;
    std::unordered_set<std::string> shaderIncludeDirs;
    std::string vertexPath;
    std::string fragmentPath;

    inline static const std::unordered_set<std::string> reservedDirectives = {
        "#version", "#define", "#undef", "#ifdef", "#ifndef",
        "#else", "#endif", "#if", "#elif", "#else", "#endif",
        "#error", "#warning", "#pragma", "#extension"
    };
    inline static const std::unordered_set<std::string> customDirectives = {
        "#include"
    };
    inline static const size_t directivesCount = customDirectives.size();

public:
    Shader() = delete;
    Shader(const char* vertPath, const char* fragPath);

    void Use() const;

    unsigned int GetShaderID() const;
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVector3(const std::string& name, const glm::vec3& vector) const;
    void SetVector4(const std::string& name, const glm::vec4& vector) const;
    void SetMatrix4x4(const std::string& name, const glm::mat4x4& matrix) const;

    inline std::string GetVertexPath() const
    {
        return vertexPath;
    }

    inline std::string GetFragmentPath() const
    {
        return fragmentPath;
    }
    
private:
    std::string Preprocess(const std::string& shaderCode);
    std::string ProcessDirective(const std::string& directive, const std::string& line);
    unsigned int CompileShader(const char* shaderCode, unsigned int type);
    void CreateShaderProgram(int idCount, ...);
    int CheckShaderCompiled(unsigned int shaderID, const std::string& name);
    int CheckProgramLinked(unsigned int shaderProgramID, const std::string& name);
    int ReadCodeFromPath(const char* path, std::string& code, bool verbose = true);
};
