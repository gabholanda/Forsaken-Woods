#include <glew.h>
#include <iostream>
#include "Skybox.h"
#include <SDL_image.h>

Skybox::Skybox(const char* posxFilename_, const char* posyFilename_, const char* poszFilename_,
	const char* negxFilename_, const char* negyFilename_, const char* negzFilename_) : skyboxTextureID(0)
{
	//this->posxFilename = posxFilename; // "this" is optional? it points to yourself, not needed since i have _
	posxFilename = posxFilename_; 
	posyFilename = posyFilename_;
	poszFilename = poszFilename_;
	negxFilename = negxFilename_;
	negyFilename = negyFilename_;
	negzFilename = negzFilename_;
	skyboxTextureID = 0;
}

Skybox::~Skybox()
{
}

bool Skybox::OnCreate()
{
	mesh = new Mesh("meshes/cube.obj");
	mesh->OnCreate();
	glGenTextures(1, &skyboxTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	SDL_Surface* textureSurface;
	int mode;

	/////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(posxFilename);
	if (textureSurface == nullptr) {
		return false;
	}
	 mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB; // ? is an if statement
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode,
		GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(posyFilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode,
		GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(poszFilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode,
		GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(negxFilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode,
		GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(negyFilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode,
		GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(negzFilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode,
		GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	/////////////////////////////////////////////////////////////////////////////////////

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;

}

void Skybox::OnDestroy()
{
	mesh->OnDestroy();
	delete mesh;
	glDeleteTextures(1, &skyboxTextureID);
}

void Skybox::Update(const float deltaTime)
{
}

void Skybox::Render() const
{
	if (skyboxTextureID == 0) return;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	mesh->Render(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}



