#pragma once
#define MESH_H
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include<vector>

#include"Shader.h"

#include <iostream>

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int Id;
	string type;
	string path;
};
class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture>textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		setupMesh();
	}
	void Draw(Shader shader);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};


