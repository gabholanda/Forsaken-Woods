#pragma once
#include <vector>
#include <string>
#include "mesh.h"
#include "Shader.h"
#include "Trackball.h"
class Skybox {
private:
	const char* posxFilename;
	const char* posyFilename;
	const char* poszFilename;
	const char* negxFilename;
	const char* negyFilename;
	const char* negzFilename;
	Shader* shader;
	Mesh* mesh;

public:

	unsigned int skyboxTextureID;
	Skybox(const char* posxFilename_, const char* posyFilename_, const char* poszFilename_,
		const char* negxFilename_, const char* negyFilename_, const char* negzFilename_);
	~Skybox();

	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void Render() const;
};

