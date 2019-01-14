#pragma once

/*
Refs:
ModelObject - aiScene		: http://assimp.sourceforge.net/lib_html/structai_scene.html
ModelNode - aiNode			: http://assimp.sourceforge.net/lib_html/structai_node.html
AnimationNode - aiAnimNode	: http://assimp.sourceforge.net/lib_html/structai_node_anim.html
Animation - aiAnimation		: http://assimp.sourceforge.net/lib_html/structai_animation.html#a281d81ba18c79086cb55df395379bb2a
*/

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <assimp/scene.h> 

#include "Mesh.h"
#include "Material.h"

/*
	A namespace for organizing how our models are structured 
*/
namespace Model {

	/*
		Represents a node in our object hierarchy along with its children and which meshes it owns
	*/
	struct Node {
		std::string name;
		unsigned childrenCount;
		unsigned meshCount;

		Node** children; // <- should probably try to keep these together in memory
		unsigned* meshIndices;
		std::vector<unsigned> materialIndices; //Should line up one per mesh, vector until blenderized
		glm::mat4 transformMatrix;

		//Construct Object Node and allocate space for indicies and children
		Node(aiNode* aNode);
		~Node(); 
	};

	/*
		Represents a model as a whole, including its hierarchy and meshes
	*/
	struct Object {
		
		unsigned meshCount;

		//If I get blender Materials to work, make it all local in memory
		//For now since we're manually creating materials, we use a vector and just add them in
		//Owns the Material Data once added
		std::vector<Material::Data*> materials; //Hey we didn't even use this in the end

		Mesh::Data** meshes; // <- we should probably keep all meshes together in memory
		Node* root;
		

		Object(const aiScene* aScene);
		~Object();

		//Useful for quickly getting a node without navigating the tree
		std::unordered_map<std::string, Node*> namedNodes;
		void addNamedNode(std::string name, Node* node);
		Node* getNamedNode(std::string name);

	};
}