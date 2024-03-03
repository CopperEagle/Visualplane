#ifndef SHADERCLASS_H
#define SHADERCLASS_H

#include "../../../dependencies/glad.h"
#include "UniformKeeper.h"
#include <string>
#include <map>


enum class SLProgramType{Program, VertexShader, FragmentShader};

std::string get_file_contents(const char* filename);
std::string SLProgramTypeToString(SLProgramType);

class Shader
{
public:

	GLuint ID;

	Shader();
	Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
	Shader(std::string vertexShaderFile, std::string fragmentShaderFile);

	void activate();
	void registerUniform(std::string name);
	void setUniformValueF(std::string name, float value);
	void setUniformValue4F(std::string name, float* value);
	void setUniformValueI(std::string name, int value);
	void setUniformValue4fv(std::string name, const float* value);
	void initialize(const char* vertexShaderFile, const char* fragmentShaderFile);
	void del();

	private:
		UniformKeeper uKeeper;
		void compileErrors(unsigned int shader, SLProgramType type);
		void compile(const char* vertexShaderFile, const char* fragmentShaderFile);
	
};

#endif // SHADERCLASS_H
