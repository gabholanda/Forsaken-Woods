#include "Body.h"
#include <MMath.h>
Body::Body() : mesh{ nullptr }, texture{ nullptr } {
	// TODO:
	// Set up rotational inertia matrix to look like 
	// the one in the assignment doc for rotating about it's edge
	// main diagonal of Scott's Matrix3 is
	// rotationalInertia[0] =  
	// rotationalInertia[4] = 
	// rotationalInertia[8] = 

}

Body::~Body() {}

void Body::Update(float deltaTime) {
	UpdatePos(deltaTime);
	UpdateVel(deltaTime);
}

void Body::ApplyForce(Vec3 force) {
	accel = force / mass;
}

void Body::ApplyTorque(Vec3 torque)
{
	// calculates angular acceleration using torque and rotational inertia
	angularAcc = MMath::inverse(rotationalInertia) * torque;
}

void Body::UpdateOrientation(float deltaTime)
{
	// angular velocity = angle / time
	// angle = angular velocity * time
	float angleRadians = VMath::mag(angularVel) * deltaTime;
	float angleDegrees = angleRadians * RADIANS_TO_DEGREES;
	Vec3 axisOfRotation = VMath::normalize(angularVel);
	// You will learn about this with Rachida
	Quaternion rotation = QMath::angleAxisRotation(angleDegrees, axisOfRotation);
	// Update the orientation by multiplying
	orientation = orientation * rotation;
}



void Body::UpdatePos(float deltaTime)
{
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;
}

void Body::UpdateVel(float deltaTime)
{
	vel += accel * deltaTime;
}

void Body::RodConstraint(float deltaTime, Vec3 anchorPoint, float rodLength)
{
}

void Body::StraightLineConstraint(float deltaTime, float slope, float yIntercept)
{
	// We worked all this out on OneNote in class
	// Start with the position constraint
	float positionConstraint = -slope * pos.x + pos.y + yIntercept;

	// Let's build the bias term b
	float baumgarteStabilizationParameter = 0.1; // you can mess with this number
	// make sure deltaTime is not zero (this does happen if you have a breakpoint)
	if (deltaTime < VERY_SMALL) {
		return;
	}
	float b = (baumgarteStabilizationParameter * positionConstraint) / deltaTime;

	// Build Jacobian * velocity
	// That is just the velocity constraint
	float JV = -slope * vel.x + vel.y;

	// Build the denominator of the Lagrangian multiplier
	// aka the inverse effective mass J M^(-1)J^T
	float inverseEffectiveMass = (slope * slope + 1) / mass;

	// Build the Lagrangian multiplier
	float lagrangianMultiplier = (-JV - b) / inverseEffectiveMass;

	// Build the change in velocity
	// Need the transpose of the Jacobian
	Vec3 JT(-slope, 1.0f, 0.0f);
	Vec3 deltaVel = JT * (1.0f / mass) * lagrangianMultiplier;

	// Update the velocity
	vel += deltaVel;
}

void Body::CircleConstraint(float deltaTime, float constraintRadius, Vec3 circleCentre)
{
	Vec3 radialVector = pos - circleCentre;
	// We figured this out on OneNote
	float positionConstraint = (radialVector.x * radialVector.x) + (radialVector.y * radialVector.y) - (constraintRadius * constraintRadius);

	// Let's build the bias term b
	float baumgarteStabilizationParameter = 0.1; // you can mess with this number
	// make sure deltaTime is not zero (this does happen if you have a breakpoint)
	if (deltaTime < VERY_SMALL) {
		return;
	}
	float b = (baumgarteStabilizationParameter * positionConstraint) / deltaTime;

	// Calculate the Jacobian multiplied with the velocity vector
	// i.e. the velocity constraint
	float JV = 2.0f * radialVector.x * vel.x + 2.0f * radialVector.y * vel.y;

	// Find the denominator of the lagrangian multiplier (lambda)
	float inverseEffectiveMass = (4.0f * radialVector.x * radialVector.x + 4.0f * radialVector.y * radialVector.y) / mass;

	// Build the Lagrangian multiplier
	float lagrangianMultiplier = (-JV - b) / inverseEffectiveMass;

	// Build the change in velocity
	// Need the transpose of the Jacobian
	Vec3 JT(2.0f * radialVector.x, 2.0f * radialVector.y, 0.0f);

	Vec3 deltaVel = JT * (1.0f / mass) * lagrangianMultiplier;

	// Update the velocity
	vel += deltaVel;
}


void Body::SphereConstraint(float deltaTime, float constraintRadius, Vec3 sphereCentre)
{
}

Vec3 Body::SpringForce(float springLength, Vec3 anchorPoint)
{
	const float k = 20.0f;
	// Step one from my scribbles on OneNote
	Vec3 totalDisplacement = pos - anchorPoint;

	// Step two
	Vec3 springLengthVector = springLength * VMath::normalize(totalDisplacement);

	// Step three
	// Find the displacement from equilibrium. Hooke called this x
	Vec3 xVector = totalDisplacement - springLengthVector;

	// Step four. Return Hookes law
	return -k * xVector;
}

void Body::UpdateAngularVel(float deltaTime)
{
	// looks very similar to UpdateVel
	angularVel += angularAcc * deltaTime;
}

bool Body::OnCreate() {
	return true;
}

void Body::OnDestroy() {
}

void Body::Render() const {
}

