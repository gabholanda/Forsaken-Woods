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
}

void PlayerCamera::OnDestroy()
{

}