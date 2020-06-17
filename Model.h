#include <glad/glad.h>
#include <glm/glm.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

#include"Shader.h"
#include"Mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Model
{
public:
	void Draw(Shader shader);
	string directory;
	bool gammaCorrection;
	void loadModel(string const &path);
private:
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;
	
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const string& path, const string& directory);
};


