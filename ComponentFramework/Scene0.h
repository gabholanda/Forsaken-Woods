#ifndef Scene0_H
#define Scene0_H
#include "Scene.h"
#include "Vector.h"
#include <Matrix.h>
using namespace MATH;

/// Forward declarations 
// same as #include?
union SDL_Event;
class Body;
class Mesh;
class Shader;
class Texture;

class Scene0 : public Scene {
private:
	Body* skull;
	Body* lEye;
	Shader* shader;
	Mesh* skullMesh;
	Texture* skullTexture;
	Texture* moonTexture;
	Mesh* cubeMesh;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 skullModelMatrix;
	Matrix4 eyeModelMatrix;
	Matrix4 modelMatrixPlatform1;
	Matrix4 modelMatrixPlatform2;
	Matrix4 modelMatrixPlatform3;
	float posy;
	float platformAngleDegrees = 25.0f;
	float platformAngleRadians = platformAngleDegrees * DEGREES_TO_RADIANS;
	Vec3 torque;
	Vec3 lightPos;
	Vec3 lightPos2;
	bool drawInWireMode;

public:
	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE0_H