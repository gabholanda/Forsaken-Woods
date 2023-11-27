#include "UIText.h"
#include "SDL.h"

UIText::UIText(const char* text_, int fontSize, const char* fontName, SDL_Renderer* renderer_, Vec2 position_, SDL_Color color_ = { 255,255,255 })
{
	font = TTF_OpenFont(fontName, fontSize);
	if (!font)
	{
		cout << "Failed to load Font" << endl;
		return;
	}
	renderer = renderer_;
	color = color_;
	position = position_;
	text = TTF_RenderText_Solid(font, text_, color);;
	texture = SDL_CreateTextureFromSurface(renderer, text);
}

void UIText::setText(const char* text_)
{
	text = TTF_RenderText_Solid(font, text_, color);
	texture = SDL_CreateTextureFromSurface(renderer, text);
}

void UIText::Render()
{
	SDL_Rect dest = { position.x, position.y, text->w, text->h };

	SDL_RenderCopy(renderer, texture, NULL, &dest);
	SDL_FreeSurface(text);
	SDL_DestroyTexture(texture);
}


