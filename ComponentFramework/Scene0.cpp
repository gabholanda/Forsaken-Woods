#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene0.h"
#include <MMath.h>
#include "Debug.h"
#include "Mesh.h"
#include "Shader.h"
#include "Body.h"
#include "Texture.h"

Scene0::Scene0() :skull{ nullptr }, lEye{ nullptr }, shader { nullptr }, skullMesh{ nullptr }, cubeMesh{ nullptr },
drawInWireMode{ true } {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
}

bool Scene0::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	skull = new Body();
	skull->OnCreate();
	lEye = new Body();
	lEye->OnCreate();
	skull->setRadius(1);


	float angularSpeedDegrees = 10.0f;
	float angularSpeedRadians = angularSpeedDegrees * DEGREES_TO_RADIANS;

	skull->axisOfRotation = Vec3(0.0f, 1.0f, 0.0f);
	skull->angularVel = angularSpeedRadians * skull->axisOfRotation;
	skull->pos.set(0, 0, 0);
	lEye->axisOfRotation = Vec3(0.0f, 1.0f, 0.0f);
	lEye->angularVel = angularSpeedRadians * skull->axisOfRotation;
	lEye->pos.set(0, 0, 0);
	skullMesh = new Mesh("meshes/sphere.obj");
	skullMesh->OnCreate();



	shader = new Shader("shaders/phongVert.glsl", "shaders/phongFrag.glsl");
	if (shader->OnCreate() == false) {
		std::cout << "Shader failed ... we have a problem\n";
	}


	skullTexture = new Texture();
	skullTexture->LoadImage("./textures/earthclouds.jpg");

	moonTexture = new Texture();
	moonTexture->LoadImage("./textures/moon.jpg");

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	skullModelMatrix.loadIdentity();
	eyeModelMatrix.loadIdentity();

	lightPos = Vec3(10.0f, 0.0f, 0.0f);
	return true;
}

void Scene0::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	skull->OnDestroy();
	delete skull;

	lEye->OnDestroy();
	delete lEye;


	skullMesh->OnDestroy();
	delete skullMesh;


	shader->OnDestroy();
	delete shader;

	delete skullTexture;
	delete moonTexture;


}

void Scene0::HandleEvents(const SDL_Event& sdlEvent) {
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			drawInWireMode = !drawInWireMode;
			break;
		}
		break;

	case SDL_MOUSEMOTION:
		break;

	case SDL_MOUSEBUTTONDOWN:
		break;

	case SDL_MOUSEBUTTONUP:
		break;

	default:
		break;
	}
}

void Scene0::Update(const float deltaTime) {

	static float totalTime = 0.0f;
	totalTime += deltaTime;
	skullModelMatrix = MMath::rotate(totalTime * 12.0f, Vec3(0.0f, 1.0f, 0.0f)) * MMath::rotate(-90.0f, Vec3(1.0f, 0.0f, 0.0f));
	eyeModelMatrix = MMath::rotate(totalTime * 6.5, Vec3(0.0f, 1.0f, 0.0f)) * MMath::rotate(-90.0f, Vec3(1.0f, 0.0f, 0.0f)) * MMath::scale(0.25, 0.25, 0.25) * MMath::translate(15.0f, 0.0f, 0.0f);
}

void Scene0::Render() const {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	/// Set the background color then clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (drawInWireMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(shader->GetUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, skullModelMatrix);
	glUniform3fv(shader->GetUniformID("lightPos"), 1, lightPos);
	glBindTexture(GL_TEXTURE_2D, skullTexture->getTextureID());
	skullMesh->Render(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, eyeModelMatrix);

	glBindTexture(GL_TEXTURE_2D, moonTexture->getTextureID());
	skullMesh->Render(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, 0);


	glUseProgram(0);
}




