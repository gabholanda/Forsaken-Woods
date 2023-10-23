#ifndef BODY_H
#define BODY_H
#include "GameObject.h"
#include "IRender.h"
#include <MMath.h>
#include <VMath.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace MATH;

class Randomizer;

class Body : public GameObject, public IRender {
protected:
	// inherited classes can access this
	Vec3 vel;
	Vec3 accel;
	Vec3 size;           // box size
	float mass;
	float rotation;			// rotating at this speed (radians per second?)
	float angular;          // angular acceleration
	float movementSpeed;
	float scale;

	Vec3 imageSizeWorldCoords;
	SDL_Surface* image;
	SDL_Texture* texture;



public:
	Body();
	Body(
		Vec3 pos_, Vec3 vel_, Vec3 accel_,
		Vec3 size_,
		float mass_,
		float orientation_,
		float rotation_,
		float angular_,
		float movementSpeed_,
		float scale_
	);
	virtual ~Body();
	virtual void Update(float deltaTime);
	virtual void ApplyForce(Vec3 force_);
	virtual Vec3 getVel() { return vel; }
	virtual Vec3 getAccel() { return accel; }
	virtual Vec3 getSize() { return size; }
	virtual float getMass() { return mass; }
	virtual float getRotation() { return rotation; }
	virtual float getAngular() { return angular; }
	virtual float getScale() { return scale; }
	virtual float getMovementSpeed() { return movementSpeed; }
	virtual void setMovementSpeed(float movementSpeed_) { movementSpeed = movementSpeed_; }

	virtual void HandleEvents(const SDL_Event& event);

};

#endif /* BODY_H */
