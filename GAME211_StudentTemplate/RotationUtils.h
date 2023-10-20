#pragma once
#include <MMath.h>
#include <SDL.h>
using namespace MATH;

static class RotationUtils
{
public:
	static void RotateTowardsMouse(float &outOrientation, Matrix4 projectionMatrix, Vec3 pos);
	static void RotateTowardsTarget(float &outOrientation, Vec3 targetPos, Vec3 pos);
};

