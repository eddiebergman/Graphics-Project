#include "ModelImporter.h"
#include <assimp/cimport.h> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Model.h"

//Model storage
std::unordered_map<std::string, Model::Object*> models;

//Loads in all the models, saving them to be accessed based on value of key:value
void ModelImporter::loadModels(std::unordered_map<std::string, std::string> modelsToLoad)
{
	for (auto it = modelsToLoad.begin(); it != modelsToLoad.end(); ++it)
		loadModel(it->first, it->second);
}

void ModelImporter::loadModel(std::string name, std::string location) {

	//Flags:
	//http://assimp.sourceforge.net/lib_html/postprocess_8h.html#a64795260b95f5a4b3f3dc1be4f52e410
	const aiScene* scene = aiImportFile(location.c_str(),
		aiProcess_Triangulate | 
		aiProcess_JoinIdenticalVertices
	);
	if (!scene){
		fprintf(stderr, "ERROR: reading mesh %s\n", location.c_str());
		exit(0);
	}

	//Convert to our Hierarchy (ModelObject)
	Model::Object* model = new Model::Object(scene);
	models.insert({ name, model });
	aiReleaseImport(scene);
}

//Returns a model based on value
Model::Object & ModelImporter::getModel(std::string name)
{
	return *models.at(name);
}