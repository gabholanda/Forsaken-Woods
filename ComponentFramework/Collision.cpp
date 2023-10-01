#include "Collision.h"
#include "VMath.h"

bool COLLISION::SpherePlaneCollisionDetected(const Body* body, const Plane& plane)
{
	float fudgeFactor = 0.1f;
	float dist = PMath::distance(body->pos, plane) - body->getRadius() - fudgeFactor;
	if (dist < 0) {
		return true;
	}
	return false;
}

void COLLISION::SpherePlaneCollisionResponse(Body* body, const Plane& plane)
{
	if (COLLISION::SpherePlaneCollisionDetected(body, plane)) {
		// Onto collision response
		// Use equation I wrote in OneNote
		Vec3 normal(plane.x, plane.y, plane.z);
		Vec3 P = VMath::dot(-body->vel, normal) * normal;
		// only bother with collision response if
		// the velocity is generally opposite the normal
		if (VMath::dot(body->vel, normal) < 0) {
			body->vel += 2.0f * P;
		}
	}
}


bool COLLISION::SphereSphereCollisionDetected(const Body* body1, const Body* body2)
{
	// TODO
	// Find the distance between the sphere centres and compare to the two radii
	// if |pos1 - pos2| less than or equal to the two radii added together, then return true
	// else return false
	float centerDistance = VMath::mag(body1->pos - body2->pos);
	float radiusDistance = body1->getRadius() + body2->getRadius();
	if (centerDistance <= radiusDistance) {
		std::cout << "collding" << std::endl;
		return true;
	}
	return false;
}

void COLLISION::SphereSphereCollisionResponse(Body* body1, Body* body2)
{
	if (SphereSphereCollisionDetected(body1, body2)) {
		float E = 0.5;
	
		Vec3 cNormal = VMath::normalize(body1->pos - body2->pos);
		float vRel = VMath::dot((body1->vel - body2->vel), cNormal);
		if (vRel < 0) {
			float J = (-(1 + E) * vRel) / ((1 / body1->getMass()) + (1 / body2->getMass()));
			body1->vel += (J * cNormal) / body1->getMass();
			body2->vel -= (J * cNormal) / body2->getMass();							
		}
	}
}	
