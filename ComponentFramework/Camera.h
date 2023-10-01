#pragma once
#include <MMath.h>
#include "Skybox.h"
#include "Shader.h"
#include "Mesh.h"
using namespace MATH;
class Camera
{
private:
	int viewport[4];
	float aspectRatio;
	Matrix4 projection;
	Matrix4 view;
	Skybox* skybox;
	Shader* shader;
	Mesh* mesh;

public:
	Trackball* trackball;
	Camera();
	~Camera();

	void Perspective(float fovy, float aspectRatio, float near, float far);
	void LookAt(const Vec3 &eye, const Vec3 &at, const Vec3 &up);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);
	Matrix4 getProjection() { return projection; }
	/*void setProjection(Matrix4 projection_) {
		projection = projection_;
	};*/

	Matrix4 getView() { return view; }
	/*void setView(Matrix4 view_) {
		projection = view_;
	};*/

	Skybox* getSkybox() const { return skybox; };
};

