#include "Model.h"

glm::mat4 convert(const aiMatrix4x4 m);

namespace Model{
	
	void addChildrenToMap(Model::Node* node, std::unordered_map<std::string, Model::Node*>& map);

	//Node
	Node::Node(aiNode* aNode)
		: name(aNode->mName.C_Str()), childrenCount(aNode->mNumChildren), meshCount(aNode->mNumMeshes),
		transformMatrix(convert(aNode->mTransformation))
	{
		//Allocate space
		children = (Node**)malloc(childrenCount * sizeof(Node*));
		meshIndices = (unsigned*)malloc(meshCount * sizeof(unsigned));
		
		
		//Add all meshes
		for (unsigned i = 0; i < meshCount; i++)
			this->meshIndices[i] = aNode->mMeshes[i];

		//If has children, insert them too
		for (unsigned i = 0; i < childrenCount; i++)
			children[i] = new Node(aNode->mChildren[i]);
	}

	Node::~Node() {
		delete[] children;
		delete[] meshIndices;
	}
	//end Node

	//Object
	Object::Object(const aiScene* scene)
		: meshCount(scene->mNumMeshes)
	{
		bool lala = scene->HasAnimations();
		//Allocate space
		meshes = (Mesh::Data**)malloc(meshCount * sizeof(Mesh::Data*));

		//Transfer Meshes over
		for (unsigned i = 0; i < meshCount; i++)
			meshes[i] = new Mesh::Data(scene->mMeshes[i]);

		//Create tree of Nodes
		root = new Node(scene->mRootNode);

		//Add all named children to its map
		addChildrenToMap(root, namedNodes);
	}

	Object::~Object() {
		delete[] meshes;
		delete root;
	}
	
	void Object::addNamedNode(std::string name, Node * node)
	{
		namedNodes.insert({name, node});
	}
	
	Node * Object::getNamedNode(std::string name)
	{
		return namedNodes.at(name);
	}
	//end Object

	void addChildrenToMap(Node* node, std::unordered_map<std::string, Node*>& map) {
		map.insert({ node->name, node });
		for (unsigned i = 0; i < node->childrenCount; i++)
			addChildrenToMap(node->children[i], map);
	}
}

glm::mat4 convert(const aiMatrix4x4 m)
{
	glm::mat4 mat;

	mat[0][0] = m.a1;
	mat[0][1] = m.b1;
	mat[0][2] = m.c1;
	mat[0][3] = m.d1;
	mat[1][0] = m.a2;
	mat[1][1] = m.b2;
	mat[1][2] = m.c2;
	mat[1][3] = m.d2;
	mat[2][0] = m.a3;
	mat[2][1] = m.b3;
	mat[2][2] = m.c3;
	mat[2][3] = m.d3;
	mat[3][0] = m.a4;
	mat[3][1] = m.b4;
	mat[3][2] = m.c4;
	mat[3][3] = m.d4;

	return mat;
}
