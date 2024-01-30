#include "engine_pch.h"
#include "Shader.h"

#include "glad/glad.h"
#include "logger/Logger.h"

Shader::Shader(const char* vertPath, const char* fragPath)
{
    std::string vertexFile   = vertPath;
    std::string fragmentFile = fragPath;

	const std::string vertexDir   = vertexFile.substr(0, vertexFile.find_last_of("/\\") + 1);
	const std::string fragmentDir = fragmentFile.substr(0, fragmentFile.find_last_of("/\\") + 1);

	shaderIncludeDirs.insert(vertexDir);
	shaderIncludeDirs.insert(fragmentDir);
    
    vertexFile   = vertexFile.substr(vertexFile.find_last_of("/\\") + 1);
    fragmentFile = fragmentFile.substr(fragmentFile.find_last_of("/\\") + 1);
    
	std::string vCode;
	std::string fCode;

	ReadCodeFromPath(vertPath, vCode);
	ReadCodeFromPath(fragPath, fCode);

	vCode = Preprocess(vCode);
	fCode = Preprocess(fCode);

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


std::string Shader::Preprocess(const std::string& shaderCode)
{
	std::istringstream codeStreamInput(shaderCode);
	std::ostringstream codeStreamOutput;

	std::string line;
	while(std::getline(codeStreamInput, line))
	{
		if (!line.starts_with("#"))
		{
			codeStreamOutput << line << "\n";
			continue;
		}

		std::istringstream lineStream(line);
		std::string currentDirective;
		lineStream >> currentDirective;
		
		if (customDirectives.contains(currentDirective))
		{
			codeStreamOutput << ProcessDirective(currentDirective, line);
		}
		else
		{
			if (!reservedDirectives.contains(currentDirective))
			{
				const std::string warnMsg = "Unknown shader preprocessor directive: " + line + ". Continuing without modification...";
				Logger::Log(LogCategory::WARNING, warnMsg, "Shader::Preprocess");
			}
			
			codeStreamOutput << line << "\n";
		}
	}

	return codeStreamOutput.str();
}

std::string Shader::ProcessDirective(const std::string& directive, const std::string& line)
{
	std::istringstream lineStream(line);
	std::vector<std::string> args;

	std::string arg;
	lineStream >> arg; // Ignore the directive
	while (lineStream >> arg)
	{
		args.push_back(arg);
	}

	std::string ret;

	if (directive == "#include")
	{
		if (args.size() != 1)
		{
			Logger::Log(LogCategory::ERROR, "Incorrect number of arguments in #include directive: " + line, "Shader::ProcessDirective");

			return line;
		}

		bool fileFound = false;
		for (const std::string& includeDir : shaderIncludeDirs)
		{
			// Remove single/double quotes from the filename
			std::erase(args[0], '\"');
			std::erase(args[0], '\'');
			
			std::string path = includeDir + args[0];
			if (ReadCodeFromPath(path.c_str(), ret, false) == 0)
			{
				ret = Preprocess(ret);
				fileFound = true;
				break;
			}
		}

		if (!fileFound)
		{
			Logger::Log(LogCategory::ERROR, "Unable to locate include file: " + line, "Shader::ProcessDirective");

			return line;
		}
	}

	return ret;
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

int Shader::ReadCodeFromPath(const char* path, std::string& code, bool verbose)
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
		if (verbose)
		{
			std::string errorMsg = path;
			errorMsg += "\n";
			errorMsg += err.what();
		
			Logger::Log(LogCategory::ERROR, errorMsg, "Shader::ReadCodeFromPath");
		}
		
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
