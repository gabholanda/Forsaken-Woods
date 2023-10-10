#include "PlayerCamera.h"


void PlayerCamera::updateCameraPosition()
{
	//projectionMatrix = MMath::translate(manager->getPlayer()->getPos());
	ortho = MMath::orthographic(0.0f, 2 * manager->getPlayer()->getPos().x, 0.0f, 2 * manager->getPlayer()->getPos().y, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
}

void PlayerCamera::OnDestroy()
{

}