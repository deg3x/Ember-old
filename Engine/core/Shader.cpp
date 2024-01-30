#include "engine_pch.h"
#include "Shader.h"

#include "glad/glad.h"
#include "logger/Logger.h"

Shader::Shader(const char* vertPath, const char* fragPath)
{
    std::string vertexFile   = vertPath;
    std::string fragmentFile = fragPath;
    
    vertexFile   = vertexFile.substr(vertexFile.find_last_of("/\\") + 1);
    fragmentFile = fragmentFile.substr(fragmentFile.find_last_of("/\\") + 1);
    
	std::string vCode;
	std::string fCode;

	ReadCodeFromPath(vertPath, vCode);
	ReadCodeFromPath(fragPath, fCode);

	const char* vertCode = vCode.c_str();
	const char* fragCode = fCode.c_str();

	const unsigned int vertID = CompileShader(vertCode, GL_VERTEX_SHADER);
	const unsigned int fragID = CompileShader(fragCode, GL_FRAGMENT_SHADER);

	CreateShaderProgram(2, vertID, fragID);
}

void Shader::Use() const
{
	glUseProgram(programID);
}

unsigned int Shader::CompileShader(const char* shaderCode, unsigned int type)
{
	const unsigned int shaderID = glCreateShader(type);

	glShaderSource(shaderID, 1, &shaderCode, nullptr);
	glCompileShader(shaderID);

	CheckShaderCompiled(shaderID, shaderCode);

	return shaderID;
}

void Shader::CreateShaderProgram(int idCount, ...)
{
	programID = glCreateProgram();

	va_list shaderIDs;
	va_start(shaderIDs, idCount);

	for (int i = 0; i < idCount; i++)
	{
		glAttachShader(programID, va_arg(shaderIDs, unsigned int));
	}

	glLinkProgram(programID);
	CheckProgramLinked(programID, "SHADER_PROGRAM");

	va_list shaderIDsCopy;
	va_copy(shaderIDsCopy, shaderIDs);

	for (int i = 0; i < idCount; i++)
	{
		glDeleteShader(va_arg(shaderIDsCopy, unsigned int));
	}

	va_end(shaderIDsCopy);
	va_end(shaderIDs);
}

int Shader::ReadCodeFromPath(const char* path, std::string& code)
{
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(path);

		std::stringstream codeStream;
		codeStream << file.rdbuf();

		file.close();

		code = codeStream.str();
	}
	catch (const std::ifstream::failure& err)
	{
		std::string errorMsg = path;
		errorMsg += "\n";
		errorMsg += err.what();
		
		Logger::Log(LogCategory::ERROR, errorMsg, "Shader::ReadCodeFromPath");
		return -1;
	}

	return 0;
}

int Shader::CheckShaderCompiled(unsigned int shaderID, const std::string& name)
{
	int success;
	char info[512];

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, info);
        
        const std::string errorMsg = "Shader failed to compile: " + name + "\n" + info;
		Logger::Log(LogCategory::ERROR, errorMsg, "Shader::CheckShaderCompiled");
	}

	return success;
}

int Shader::CheckProgramLinked(unsigned int shaderProgramID, const std::string& name)
{
	int success;
	char info[512];

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, NULL, info);
        
        const std::string errorMsg = "Shader failed to link: " + name + "\n" + info;
		Logger::Log(LogCategory::ERROR, errorMsg, "Shader::CheckProgramLinked");
	}

	return success;
}

unsigned int Shader::GetShaderID() const
{
	return programID;
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glProgramUniform1i(programID, glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glProgramUniform1i(programID, glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glProgramUniform1f(programID, glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetVector3(const std::string& name, const glm::vec3& value) const
{
	glProgramUniform3fv(programID, glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void Shader::SetVector4(const std::string& name, const glm::vec4& value) const
{
	glProgramUniform4fv(programID, glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

void Shader::SetMatrix4x4(const std::string& name, const glm::mat4x4& value) const
{
	glProgramUniformMatrix4fv(programID, glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
