#include "Sphere.h"

#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "Colour.h"

Mesh::Data& getMesh(Model::Object& model, std::string nodeName) {
	return *model.meshes[model.getNamedNode(nodeName)->meshIndices[0]];
}

Sphere::Sphere(Model::Object& model, Light::Spot spotlight)
	: rModel(model), mSpotlight(spotlight)
{
	//Add inner rings
	mMiddleRingInners.reserve(RING_INNERS);
	for (int i = 1; i <= RING_INNERS; i++) {
		Model::Node& node = *rModel.getNamedNode("MiddleRingInner" + std::to_string(i));
		Mesh::Data& mesh = *rModel.meshes[node.meshIndices[0]];
		Material::Data& material = Material::get("defaultSphereFace");
		mMiddleRingInners.emplace_back(node, mesh, material, createPointLight(node, material));
	}
	
	//Add Note faces
	mNoteFaces.reserve(Note::NOTE_COUNT);
	for (int i = 0; i != Note::NOTE_COUNT; i++) {
		Model::Node& node = *rModel.getNamedNode(Note::stringFromIndex(i));
		Mesh::Data& mesh = *rModel.meshes[node.meshIndices[0]];
		Material::Data& material = Material::get("defaultSphereFace");
		Material::Data& colourMat = Material::get(Note::stringFromIndex(i));
		mNoteFaces.try_emplace(Note::noteFromIndex(i), node, mesh, material, createPointLight(node, colourMat));
	}

	//Add the Note faces point lights
	for (auto& noteFace : mNoteFaces) {
		int id = Light::Point::addLight(noteFace.second.light);
		noteFace.second.pointLightId = id;
		Light::Point::updateState(id, false);
	}

	//Add the inner face point lights
	for (auto& innerFace : mMiddleRingInners) {
		int id = Light::Point::addLight(innerFace.light);
		innerFace.pointLightId = id;
		Light::Point::updateState(id, false);
	}
	
	//Add the light
	mLightId = Light::Spot::addLight(mSpotlight);
}


Sphere::~Sphere()
{
}

void Sphere::update(glm::mat4 parentMatrix)
{
	for (auto it = mNoteFaceAnimations.begin(); it != mNoteFaceAnimations.end();) {
		if (it->isDone()) {
			it->finished();
			it = mNoteFaceAnimations.erase(it);
		}
		else {
			it->animate(parentMatrix);
			++it;
		}
	}
	for (auto it = mRingAnimations.begin(); it != mRingAnimations.end();) {
		if (it->isDone()) {
			it->finished();
			it = mRingAnimations.erase(it);
		}
		else {
			++it;
		}
	}
	for (auto it = mSphereAnimations.begin(); it != mSphereAnimations.end();) {
		if (it->isDone()) {
			it->finished();
			it = mSphereAnimations.erase(it);
		}
		else {
			++it;
		}
	}
}

void Sphere::draw(Shader & shader, glm::mat4 parentMatrix)
{
	shader.setUniformMat4(Shader::UNIFORM_MODEL, parentMatrix);
	//Draw each subsection of the sphere

	glm::mat4 modifiedParentMatrix = parentMatrix;
	//Animate with sphereAnimations
	for (auto& it : mSphereAnimations) {
		modifiedParentMatrix = it.animate(modifiedParentMatrix);
	}

	//Note faces
	for (auto& it : mNoteFaces) {
		shader.setUniformMat4(Shader::UNIFORM_MODEL, modifiedParentMatrix * it.second.modelNode.transformMatrix);
		shader.setMaterial(it.second.activeMaterial);
		glBindVertexArray(it.second.mesh.VAO);
		glDrawElements(GL_TRIANGLES, it.second.mesh.points(), GL_UNSIGNED_INT, 0);
	}

	//Animate with ringAnimations
	for (auto& it : mRingAnimations) {
		modifiedParentMatrix = it.animate(modifiedParentMatrix);
	}

	//Middle Ring
	for (auto& it : mMiddleRingInners) {
		shader.setUniformMat4(Shader::UNIFORM_MODEL, modifiedParentMatrix * it.modelNode.transformMatrix);
		shader.setMaterial(it.activeMaterial);
		glBindVertexArray(it.mesh.VAO);
		glDrawElements(GL_TRIANGLES, it.mesh.points(), GL_UNSIGNED_INT, 0);
	}



}

void Sphere::activateNote(Note::T note, int amount)
{
	//Change our spotlight colour
	Material::Data& activatedMaterial = Material::get(Note::stringFromNote(note));
	mSpotlight.light.ambient = activatedMaterial.diffuseColour;
	mSpotlight.light.diffuse = activatedMaterial.diffuseColour;
	mSpotlight.light.specular = { 1.0f, 1.0f, 1.0f };
	Light::Spot::updateLightValues(mLightId, mSpotlight);
	Light::Spot::updateState(mLightId, true);
	//Create new animations
	mNoteFaceAnimations.emplace_back(face(note), note, amount);
	mRingAnimations.emplace_back(Material::get(Note::stringFromNote(note)), mMiddleRingInners, amount);
	mSphereAnimations.emplace_back(amount);
}

SphereFace& Sphere::face(Note::T note) {
	return mNoteFaces.at(note);
}

Light::Point Sphere::createPointLight(const Model::Node & node, const Material::Data & material)
{
	return Light::Point{
		{0,-5,-5}, //We'll calculate its position each frame either way
		material.diffuseColour,
		material.diffuseColour,
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.00f, 0.020f }
	};
}

SphereFace::SphereFace(Model::Node & modelNode, Mesh::Data & mesh
	, Material::Data activeMaterial, Light::Point light)
	: modelNode(modelNode), mesh(mesh), activeMaterial(activeMaterial), light(light)
{}

unsigned SphereNoteFaceAnimation::KEYFRAME_COUNT = 100;
SphereNoteFaceAnimation::SphereNoteFaceAnimation(SphereFace & face, Note::T note, int amount)
	: sphereFace(face), keyframe(0){
	
	//turn on the face
	face.on = true;

	//Set Faces material and light colour to match the material
	Material::Data& colourMat = Material::get(Note::stringFromNote(note));
	face.activeMaterial = colourMat;
	face.light.light = { colourMat.diffuseColour, colourMat.diffuseColour, colourMat.diffuseColour };

	//Turn its light on in the shader
	Light::Point::updateState(face.pointLightId, true);
}

bool SphereNoteFaceAnimation::isDone()
{
	return keyframe >= KEYFRAME_COUNT;
}

void SphereNoteFaceAnimation::finished()
{
	//turn face and light off
	sphereFace.on = false;
	sphereFace.activeMaterial = Material::get("defaultSphereFace");
	Light::Point::updateState(sphereFace.pointLightId, false);
}

void SphereNoteFaceAnimation::animate(glm::mat4 mat)
{
	sphereFace.light.position = glm::vec3(mat * sphereFace.modelNode.transformMatrix * glm::vec4{ 0,0,0,1 });
	sphereFace.light.light.ambient *= 0.95;
	sphereFace.light.light.diffuse *= 0.95;
	sphereFace.light.light.specular *= 0.95;
	Light::Point::updateLightValues(sphereFace.pointLightId, sphereFace.light);
	Light::Point::updatePosition(sphereFace.pointLightId, sphereFace.light);
	keyframe++;
}

unsigned RingAnimation::KEYFRAME_COUNT = 60;
RingAnimation::RingAnimation(Material::Data & material, std::vector<SphereFace>& ring, int amount)
 : keyframe(0), ring(ring), material(material)
{
	//Generate random numbers from 0,180 and get rotations per part
	float xRot = glm::radians((float) (rand() % 180)) / KEYFRAME_COUNT;
	float yRot = glm::radians((float)(rand() % 180)) / KEYFRAME_COUNT;
	float zRot = glm::radians((float)(rand() % 180)) / KEYFRAME_COUNT;
	scaling = ((float)(rand() % 100 )) / 130.0f / (float) KEYFRAME_COUNT;
	rotations = { xRot, yRot, zRot };
	for (auto& it : ring) {
		it.activeMaterial = material;
	}
}

bool RingAnimation::isDone()
{
	return keyframe >= KEYFRAME_COUNT;
}

void RingAnimation::finished()
{

}

glm::mat4 RingAnimation::animate(glm::mat4 mat)
{
	glm::vec3 rots = (keyframe < (KEYFRAME_COUNT / 2.0)
		? rotations * (float) keyframe
		: rotations * (float) (KEYFRAME_COUNT - keyframe)
	);
	float scaleValue = (keyframe < (KEYFRAME_COUNT / 2.0)
		? 1 + scaling * (float)keyframe
		: 1 + scaling * (float)(KEYFRAME_COUNT - keyframe)
		);
	
	glm::mat4 m = glm::scale(mat, scaleValue * glm::vec3{ 1.0,1.0,1.0 });
	m = glm::rotate(m, rots.x, {1.0, 0.0, 0.0});
	m = glm::rotate(m, rots.y, { 0.0, 1.0, 0.0 });
	m = glm::rotate(m, rots.z, { 0.0, 0.0, 1.0 });
	keyframe++;
	return m;
}

unsigned SphereAnimation::KEYFRAME_COUNT = 30;
SphereAnimation::SphereAnimation(int amount)
	:keyframe(0)
{
	scaling = ((float)(rand() % 5)) / 10.0f / (float)KEYFRAME_COUNT;
}

bool SphereAnimation::isDone()	
{
	return keyframe >= KEYFRAME_COUNT;
}

void SphereAnimation::finished()
{
	///we do nothingggg
}

glm::mat4 SphereAnimation::animate(glm::mat4 mat)
{
	float scaleValue = (keyframe < (KEYFRAME_COUNT / 2.0)
		? 1 + scaling * (float)keyframe*2
		: 1 + scaling * (float)(KEYFRAME_COUNT - keyframe)*2
		);
	keyframe++;
	return glm::scale(mat, scaleValue * glm::vec3{ 1.0,1.0,1.0 });
}
