#pragma once

#include <glm/mat4x4.hpp>
#include <list>

#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Material.h"
#include "Note.h"

/*
	Hmm these should all probably have been in one namespace
*/

/*
	Represents a single face of our sphere, including the light that is attached to it
*/
struct SphereFace {

	Mesh::Data& mesh;
	Model::Node& modelNode;

	bool on = false;
	Material::Data activeMaterial;
	int pointLightId;
	Light::Point light;

	SphereFace(Model::Node& modelNode, Mesh::Data& mesh, 
		Material::Data startingMaterial, Light::Point light);

};

/*
	A struct for keeping track of an animation to be performed on the sphere
*/
struct SphereAnimation {

	static unsigned KEYFRAME_COUNT;
	unsigned keyframe;
	float scaling;

	SphereAnimation(int amount);

	bool isDone();
	void finished();
	glm::mat4 animate(glm::mat4 mat);

};


/*
	A struct for keeping track of an animation to be performed on the ring of the sphere
*/
struct RingAnimation{
	static unsigned KEYFRAME_COUNT;
	unsigned keyframe;

	std::vector<SphereFace>& ring;
	Material::Data& material;
	glm::vec3 rotations;
	float scaling;

	RingAnimation(Material::Data& material, std::vector<SphereFace>& ring, int amount);

	bool isDone();
	void finished();
	glm::mat4 animate(glm::mat4 mat);

};

/*
	A struct for keeping track of an animation to be performed on the face of a sphere
*/
struct SphereNoteFaceAnimation {

	static unsigned KEYFRAME_COUNT;
	unsigned keyframe;
	SphereFace& sphereFace;

	SphereNoteFaceAnimation(SphereFace& face, Note::T note, int amount);

	bool isDone();
	void finished();
	void animate(glm::mat4 mat);
};

/*
	A class to keep track of all our animations, updating and drawing as well as being able to 
	inform the sphere a note has been pressed
*/
class Sphere
{
public:

	const static unsigned RING_INNERS = 10;


	Sphere(Model::Object& model, Light::Spot spotlight);
	~Sphere();

	void update(glm::mat4 parentMatrix);
	void draw(Shader& shader, glm::mat4 parentMatrix);
	void activateNote(Note::T note, int amount);

	Model::Object& rModel;
	std::unordered_map<Note::T, SphereFace> mNoteFaces;
	std::vector<SphereFace> mMiddleRingInners;
	std::list<SphereNoteFaceAnimation> mNoteFaceAnimations;
	std::list<SphereAnimation> mSphereAnimations;
	std::list<RingAnimation> mRingAnimations;
	Light::Spot mSpotlight;
	int mLightId;

private:
	SphereFace& face(Note::T note);
	Light::Point createPointLight(const Model::Node& node, const Material::Data& material);

};