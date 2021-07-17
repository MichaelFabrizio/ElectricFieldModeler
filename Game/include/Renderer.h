#pragma once
#include <glad/glad.h>
#include <string>

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Renderer
{
public:
	Renderer();
	void Draw();
private:
	void CompileShaders(const char*& filePath);
	int CompileShader(std::string shaderString, GLenum shaderType);
	ShaderProgramSource ImportShader(const std::string& filepath);

	unsigned int shaderProgram_;
};