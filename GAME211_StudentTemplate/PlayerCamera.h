#pragma once
#include "Camera.h"
#include "GameManager.h"

class PlayerCamera : public Camera
{
public:
	class GameManager* manager = NULL;
	PlayerCamera(int w, int h, float xAxis, float yAxis, GameManager* _manager)
	{
		manager = _manager;
		ndc = MMath::viewportNDC(w, h);
		ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
		projectionMatrix = ndc * ortho;
	}

	void updateCameraPosition();
	void OnDestroy();
	Vec2 getCameraBounds() const; // Get the current camera bounds (width, height)
	Vec3 getCameraCenter() const; // Get the current camera center position (player's position)
};

