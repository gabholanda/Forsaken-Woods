#include "PlayerCamera.h"


void PlayerCamera::updateCameraPosition()
{
	if (!manager)
	{
		return;
	}
	if (!manager->getPlayer())
	{
		return;
	}
	//projectionMatrix = MMath::translate(manager->getPlayer()->getPos());
	ortho = MMath::orthographic(
		manager->getPlayer()->getPos().x - manager->getSceneWidth() / 2.0f,
		manager->getPlayer()->getPos().x + manager->getSceneWidth() / 2.0f,
		manager->getPlayer()->getPos().y - manager->getSceneHeight() / 2.0f,
		manager->getPlayer()->getPos().y + manager->getSceneHeight() / 2.0f,
		0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	getCameraCenter();
}

void PlayerCamera::OnDestroy()
{

}

Vec2 PlayerCamera::getCameraBounds() const {
	float sceneWidth = manager->getSceneWidth();
	float sceneHeight = manager->getSceneHeight();
	return { sceneWidth, sceneHeight };
}

Vec3 PlayerCamera::getCameraCenter() const {
	return manager->getPlayer()->getPos();
}