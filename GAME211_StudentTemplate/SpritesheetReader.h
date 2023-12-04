#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <vector>
// Ref https://gigi.nullneuron.net/gigilabs/animations-with-sprite-sheets-in-sdl2/

using namespace std;

class SpritesheetReader
{
public:
	SpritesheetReader(int width_, int height, int rows_, int columns_);
	~SpritesheetReader()
	{
		SDL_FreeSurface(image);
		SDL_DestroyTexture(texture);
	}
	bool LoadFromFile(const char* path, SDL_Renderer* renderer);
	SDL_Texture* GetTexture() const { return texture; }
	SDL_Surface* GetImage() const { return image; }
	int GetRows() const { return rows; }
	int GetColumns() const { return columns; }
	vector<vector<SDL_Rect>> GetRects() const { return rects; }
	void SetRects();

private:
	SDL_Surface* image;
	SDL_Texture* texture;
	vector<vector<SDL_Rect>> rects;
	//Image dimensions
	int width;
	int height;
	int rows;
	int columns;
};

