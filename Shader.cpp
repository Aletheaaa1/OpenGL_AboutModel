#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.h"
#include "Render.h"

Shader::Shader(const std::string& path) : file_path(path)
{
	ShaderSource source = ReadShaderFile();
	program = CreateShader(source.vertex_shader, source.fragment_shader);
	// glUseProgram(program);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(program));
}

void Shader::Bind()
{
	glUseProgram(program);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

ShaderSource Shader::ReadShaderFile()
{
	std::ifstream file_reader(file_path);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (std::getline(file_reader, line))
	{
		if (line.find("shader") != -1)
		{
			if (line.find("vertex") != -1)
				type = Shader::VERTEX;
			if (line.find("fragment") != -1)
				type = Shader::FRAGMENT;
		}
		else
		{
			ss[static_cast<int>(type)] << line << "\n";
		}
	}
	ShaderSource source;
	source.vertex_shader = ss[static_cast<int>(ShaderType::VERTEX)].str();
	source.fragment_shader = ss[static_cast<int>(ShaderType::FRAGMENT)].str();
	return source;
}

unsigned int Shader::CreateShader(const std::string& vertex_source, const std::string& fragment_shader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(vertex_source, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragment_shader, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	return program;
}

unsigned int Shader::CompileShader(const std::string& name, unsigned int type)
{
	unsigned int shader_id = glCreateShader(type);
	const char* src = name.c_str();
	GLCall(glShaderSource(shader_id, 1, &src, nullptr));
	GLCall(glCompileShader(shader_id));

	int status;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
	while (status == 0)
	{
		int length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
		char* error = new char[length];
		glGetShaderInfoLog(shader_id, length, &length, error);
		std::cout << error << std::endl;
		delete[] error;
		return -1;
	}

	return shader_id;
}

void Shader::SetUniform4f(const std::string& name, const float* values)
{
	int location = getLocation(name);
	GLCall(glUniform4f(location, values[0], values[1], values[2], values[3]));
}

void Shader::SetUniform3f(const std::string& name, const float* values)
{
	int location = getLocation(name);
	GLCall(glUniform3f(location, values[0], values[1], values[2]));
}

void Shader::SetUniform3f(const std::string& name, std::initializer_list<float> values)
{
	int location = getLocation(name);
	float val[3];
	int index = 0;
	for (const auto& i : values)
	{
		val[index] = i;
		index++;
	}
	glUniform3f(location, val[0], val[1], val[2]);
}

void Shader::SetUniform1i(const std::string& name, const int value)
{
	int location = getLocation(name);
	GLCall(glUniform1i(location, value));
}

void Shader::SetUniform1f(const std::string& name, const float value)
{
	int location = getLocation(name);
	GLCall(glUniform1f(location, value));
}

void Shader::SetUniformMat4(const std::string& name, const float* values)
{
	int location = getLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, false, values));
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 values)
{
	int location = getLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, false, glm::value_ptr(values)));
}

void Shader::SetUniformMateria(const std::string& name, Material& material)
{
	std::string ambient = ".ambient";
	std::string diffuse = ".diffuse";
	std::string specular = ".specular";
	std::string shininess = ".shininess";

	int ambient_location = getLocation(name + ambient);
	int diffuse_location = getLocation(name + diffuse);
	int specular_loaction = getLocation(name + specular);
	int shininess_location = getLocation(name + shininess);

	const char* texture_symbol = "unsigned int";

	GLCall(glUniform3f(ambient_location, material.GetAmbient()[0], material.GetAmbient()[1], material.GetAmbient()[2]));
	GLCall(glUniform3f(diffuse_location, material.GetDiffuse()[0], material.GetDiffuse()[1], material.GetDiffuse()[2]));
	// GLCall(glUniform1i(diffuse_location, 0));
	GLCall(glUniform3f(specular_loaction, material.GetSpecular()[0], material.GetSpecular()[1],
		material.GetSpecular()[2]));
	GLCall(glUniform1f(shininess_location, material.GetShininess()));
}

void Shader::SetUniformMateriaWithMaterial(const std::string& name, Material& material)
{
	std::string ambient = ".ambient";
	std::string diffuse = ".diffuse";
	std::string specular = ".specular";
	std::string shininess = ".shininess";

	int ambient_location = getLocation(name + ambient);
	int diffuse_location = getLocation(name + diffuse);
	int specular_loaction = getLocation(name + specular);
	int shininess_location = getLocation(name + shininess);

	const char* texture_symbol = "unsigned int";

	GLCall(glUniform3f(ambient_location, material.GetAmbient()[0], material.GetAmbient()[1], material.GetAmbient()[2]));
	GLCall(glUniform1i(diffuse_location, material.GetMaterialDiffuse()));
	GLCall(glUniform1i(specular_loaction, material.GetMaterialSpecular()));
	GLCall(glUniform1f(shininess_location, material.GetShininess()));
}

int Shader::getLocation(const std::string& name)
{
	const char* n = name.c_str();
	return glGetUniformLocation(program, n);
}