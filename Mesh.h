#pragma once
#include "ShaderM.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>

struct MeshVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textCoords;
};

struct MeshTexture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	std::vector<MeshVertex> vertices;
	std::vector<MeshTexture> textures;
	std::vector<unsigned int> indices;
	Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int > indices, std::vector<MeshTexture> textures);
	void Draw(ShaderM& shader);
	unsigned int GetVao();
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	void SetupMesh();
};
