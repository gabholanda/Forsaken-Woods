#ifndef BODY_H
#define BODY_H
#include <Vector.h> /// This is mine in GameDev
#include <QMath.h>
#include <assert.h> // So we can blow up if physics info is bad
using namespace MATH; /// therefore...


/// Just forward declair these classes so I can define a pointer to them
/// Used later in the course
class Mesh;
class Texture;

class Body {
private: /// Graphics stuff 
	Mesh* mesh;
	Texture* texture;
	// Private Physics stuff
	float mass = 1.0f; // we don't want this negative or zero
	float radius = 0.5f; // we don't want this negative or zero
	Matrix3 rotationalInertia; // we don't want this negative or zero

public:
	Body();
	~Body();
	// These are linear motion variables
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	// These are rotational motion variable
	Vec3 angularVel;
	Vec3 angularAcc;
	Vec3 axisOfRotation;
	Vec3 gravity = Vec3(0.0f, 9.8f, 0.0f);
	Quaternion orientation;

	// const means "I promise this won't change member variables"
	float getRadius() const {
		return radius;
	}

	void setRadius(float radius_) {
		// Blow up if the radius_ value is not greater than zero
		// What does a zero or negative radius mean anyway?
		assert(radius_ > 0);

		radius = radius_;
		// TODO:
		// Would be a good idea to update the rotational inertia matrix 
		// if the radius (or mass) is changed

	}

	float getMass() const {
		return mass;
	}

	void setMass(float mass_) {
		// Blow up if mass is zero or negative
		assert(mass_ > 0);
		mass = mass_;
	}


	bool OnCreate();
	void OnDestroy();
	void Update(float deltaTime);
	void Render() const;
	void ApplyForce(Vec3 force);
	// Updates angular acceleration using torque and rotational inertia
	void ApplyTorque(Vec3 torque);
	// Updates orientation quaternion using the angular velocity
	void UpdateOrientation(float deltaTime);
	// Updates angular velocity using rotational equations of motion
	void UpdateAngularVel(float deltaTime);
	// Update position or velocity using the equations of motion. It's handy later on with constraints to have these as separate methods
	void UpdatePos(float deltaTime);
	void UpdateVel(float deltaTime);
	// We'll use Erin Catto 2009 GDC talk for a simple rod to build the jellyfish assignment
	// Dedicate five hours to reading his slides sloooowly
	// https ://box2d.org/files/ErinCatto_ModelingAndSolvingConstraints_GDC2009.pdf
	// and jot down all the questions you have. You will have many.
	void RodConstraint(float deltaTime, Vec3 anchorPoint, float rodLength);

	// y = mx + b type constraint
	// slope is m
	// yIntecept is b
	void StraightLineConstraint(float deltaTime, float slope, float yIntercept);

	// x^2 + y^2 = r^2 constraint
	void CircleConstraint(float deltaTime, float constraintRadius, Vec3 circleCentre);

	// Why don't you make a sphere constraint for our jellyfish
	// x^2 + y^2 +z^2 = r^2 constraint
	void SphereConstraint(float deltaTime, float constraintRadius, Vec3 sphereCentre);

	Vec3 SpringForce(float springLength, Vec3 anchorPoint);
};



#endif
