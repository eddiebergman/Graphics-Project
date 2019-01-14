#pragma once

#include <glm/vec3.hpp>
#include <unordered_map>
#include <string>

#include "Colour.h"

/*
	A namespace for keeping all our material information together so we don't pollute other classes with it
	Kind of a drop in replacement for having materials stored in Model::Object due to not being able
	to get it to work with blender
*/
namespace Material {

	struct Data{
		glm::vec3 diffuseColour;
		glm::vec3 specularColour;
		float shininess;
		
		Data(glm::vec3 diffuse, glm::vec3 specular, float shininess)
			:diffuseColour(diffuse), specularColour(specular), shininess(shininess) {}
	};

	static std::unordered_map<std::string, Data> MATERIALS {
		{ "board"	
			, { Colour::get("red"), Colour::get("white"), 1} 
		},
		{ "whiteKey"
			, { Colour::get("white"), Colour::get("white"), 1} 
		},
		{ "blackKey"
			, { Colour::get("black"), Colour::get("white"), 1} 
		},
		{ "defaultSphereFace"
			, { Colour::get("white"), Colour::get("white"), 64}
		},
		{ "C"
			, { Colour::get("C"), Colour::get("C"), 64 }
		},
		{ "Db"
			, { Colour::get("Db"), Colour::get("Db"), 64 }
		},
		{ "D"
			, { Colour::get("D"), Colour::get("D"), 64 }
		},
		{ "Eb"
			, { Colour::get("Eb"), Colour::get("Eb"), 64 }
		},
		{ "E"
			, { Colour::get("E"), Colour::get("E"), 64 }
		},
		{ "F"
			, { Colour::get("F"), Colour::get("F"), 64 }
		},
		{ "Gb"
			, { Colour::get("Gb"), Colour::get("Gb"), 64 }
		},
		{ "G"
			, { Colour::get("G"), Colour::get("G"), 64 }
		},
		{ "Ab"
			, { Colour::get("Ab"), Colour::get("Ab"), 64 }
		},
		{ "A"
			, { Colour::get("A"), Colour::get("A"), 64 }
		},
		{ "Bb"
			, { Colour::get("Bb"), Colour::get("Bb"), 64 }
		},
		{ "B"
			, { Colour::get("B"), Colour::get("B"), 64 }
		},
		{ "floor"
			, { Colour::get("white"), Colour::get("white"), 128 }
		}

	};

	static Material::Data& get(std::string materialId) {
		return MATERIALS.at(materialId);
	}

};