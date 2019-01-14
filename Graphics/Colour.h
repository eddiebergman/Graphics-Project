#pragma once

#include <glm/vec3.hpp>
#include <unordered_map>

/*
	A namespace to keep all our colours together so we don't
	end up with different colour declerations about the place.
*/
namespace Colour {

	static std::unordered_map<std::string, glm::vec3> COLOURS{
		//Note Colours
		{ "C" , {1.0, 0.0, 0.0}},
		{ "Db", {0.0, 0.0, 0.7}},
		{ "D" , {0.94, 1.0, 0.0}},
		{ "Eb", {0.6, 0.0, 0.8}},
		{ "E" , {0.0, 0.74, 0.0}},
		{ "F" , {0.8, 0.2, 0.25}},
		{ "Gb", {0.0, 0.1, 1.0}},
		{ "G" , {1.0, 0.2, 0.0}},
		{ "Ab", {0.2, 0.0, 1.0}},
		{ "A" , {0.17, 0.8, 0.0}},
		{ "Bb", {0.0, 0.8, 0.16}},
		{ "B" , {0.0, 1.0, 0.225}},

		//Others
		{ "white"	,{ 0.4,0.4,0.4 }},
		{ "red"		,{0.5, 0.0, 0.0}},
		{ "black"	,{ 0.1, 0.1 ,0.1 }}
	};

	static glm::vec3& get(std::string colourId) {
		return COLOURS.at(colourId);
	}

}