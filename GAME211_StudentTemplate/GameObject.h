#pragma once
#include <MMath.h>
#include <VMath.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace MATH;
class GameObject
{
protected:
	Vec3 pos;
	float orientation;
	float scale;

	Vec3 imageSizeWorldCoords;
	SDL_Surface* image;
	SDL_Texture* texture;
public:
	virtual void setImage(SDL_Surface* image_) { image = image_; }
	virtual SDL_Surface* getImage() { return image; }

	virtual void setImageSizeWorldCoords(Vec3 imageSizeWorldCoords_)
	{
		imageSizeWorldCoords = imageSizeWorldCoords_;
	}

	virtual void setTexture(SDL_Texture* texture_) { texture = texture_; }
	virtual SDL_Texture* getTexture() { return texture; }
	virtual Vec3 getPos() { return pos; }
	virtual float getOrientation() { return orientation; }
	virtual void setPos(Vec3 pos_) { pos = pos_; };
};

