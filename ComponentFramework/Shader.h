#pragma once

#include "glew.h"
#include <unordered_map>
#include <string>

class Shader {
	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator = (const Shader&) = delete;
	Shader& operator = (Shader&&) = delete;
private:
	const char* vsFilename;
	const char* fsFilename;
	unsigned int shaderID;
	unsigned int vertShaderID;
	unsigned int fragShaderID;
	std::unordered_map<std::string, GLuint > uniformMap;

	/// Private helper methods
	char* ReadTextFile(const char *fileName);	
	bool CompileAttach();
	bool Link();
	void SetUniformLocations();
	

public:
	Shader(const char* vsFilename_, const char* fsFilename_);
	~Shader();
	bool OnCreate();
	void OnDestroy();
	inline GLuint GetProgram() const { return shaderID;}
	GLuint GetUniformID(std::string name);
};

