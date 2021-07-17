#include "Renderer.h"
#include "Log.h"

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

Renderer::Renderer()
{
	const char* charPath = "Shader.dat";
	CompileShaders(charPath);
}

void Renderer::Draw()
{

}

void Renderer::CompileShaders(const char*& filePath)
{
	ShaderProgramSource shaders = Renderer::ImportShader(filePath);

	//Add adjustable logic for multiple shader initializations, ex: Shadow quality - Low, med, high. Etc...
	int vertexShader = CompileShader(shaders.VertexShader, GL_VERTEX_SHADER);
	int fragmentShader = CompileShader(shaders.FragmentShader, GL_FRAGMENT_SHADER);

	shaderProgram_ = glCreateProgram();

	glAttachShader(shaderProgram_, vertexShader);
	glAttachShader(shaderProgram_, fragmentShader);
	glLinkProgram(shaderProgram_);

	glUseProgram(shaderProgram_);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

int Renderer::CompileShader(std::string shaderString, GLenum shaderType)
{
	const char* shaderStringFormatted = shaderString.c_str();

	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderStringFormatted, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	ASSERT(!success, "Shader failed to compile");

	//if (!success)
	//{
	//	char infoLog[512];
	//	glGetShaderInfoLog(shader, 512, nullptr, infoLog);
	//	std::cout << "Shader failed to compile" << "[ERROR]: " << infoLog << std::endl;
	//	return -1;
	//}

	return shader;
}

//Parses the filepath and exports an array of relevant strings
ShaderProgramSource Renderer::ImportShader(const std::string& filepath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream(filepath);

	std::string line;
	std::string shaderCode;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}

			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}

			//Throw error if no valid shadertype found
			//More specifically, require at least a vertex and fragment shader (plus more possibly)
			//Quit program
			else
			{
				std::cout << "[ERROR]: Shader importation failed. No valid shadertype listed.";
			}
		}

		//Once valid shadertype line is found, write to string stream
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	//If shader enum indices change this will break, should futureproof the logic
	return { ss[0].str(), ss[1].str() };
}