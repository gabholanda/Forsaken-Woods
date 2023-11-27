#pragma once
#include "IRender.h"
#include "Vector.h"
#include <VMath.h>
#include "SDL_ttf.h"

class SDL_Surface;
class SDL_Texture;
class SDL_Renderer;

using namespace MATH;
using namespace std;

class UIText : public IRender
{
private:
	SDL_Surface* text;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Color color;
	Vec2 position;

public:
	void Render();
	UIText(const char* text_, int fontSize, const char* fontName, SDL_Renderer* renderer_, Vec2 position_, SDL_Color color_);

	~UIText()
	{
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(text);
	}

	void setText(const char* text_);
};

