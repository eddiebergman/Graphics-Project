#pragma once
#include <unordered_map>
#include "Model.h"

/*
	Imports models for us
*/
namespace ModelImporter
{
	void loadModels(std::unordered_map<std::string, std::string> modelsToLoad);
	void loadModel(std::string name, std::string location);
	Model::Object& getModel(std::string name);
};

