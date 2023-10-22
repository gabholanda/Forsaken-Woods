#include "SpritesheetReader.h"

SpritesheetReader::SpritesheetReader(int width_, int height_, int rows_, int columns_)
{
	width = width_;
	height = height_;
	rows = rows_;
	columns = columns_;
	image = nullptr;
	texture = nullptr;
}

bool SpritesheetReader::LoadFromFile(const char* path, SDL_Renderer* renderer)
{
	image = IMG_Load(path);
	texture = SDL_CreateTextureFromSurface(renderer, image);
	return image != nullptr;
}

void SpritesheetReader::SetRects()
{
	rects = vector<vector<SDL_Rect>>(columns);
	for (int i = 0; i < rects.size(); i++)
	{
		for (int j = 0; j < rows; j++)
		{
			rects[i].push_back({ j * width, i * height, width ,height });
		}
	}
}
