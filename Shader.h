#pragma once
#include <iostream>

#include "Material.h"

struct ShaderSource
{
	std::string fragment_shader;
	std::string vertex_shader;
};

class Shader
{
private:
	const std::string& file_path;
	unsigned int program;
	enum ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1,
	};

public:
	Shader(const std::string& path);
	~Shader();

	void Bind();
	void Unbind();

	void SetUniform3f(const std::string& name, const float* values);
	void SetUniform3f(const std::string& name, std::initializer_list<float> values);
	void SetUniform4f(const std::string& name, const float* values);
	void SetUniform1i(const std::string& name, const int value);
	void SetUniform1f(const std::string& name, const float value);
	void SetUniformMat4(const std::string& name, const float* values);
	void SetUniformMat4(const std::string& name, glm::mat4 values);

	void SetUniformMateria(const std::string& name, Material& material);
	void SetUniformMateriaWithMaterial(const std::string& name, Material& material);

private:
	ShaderSource ReadShaderFile();
	unsigned int CreateShader(const std::string& vertex_source, const std::string& fragment_shader);
	unsigned int CompileShader(const std::string& source, unsigned int type);

	int getLocation(const std::string& name);
};