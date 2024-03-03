#include "Shader.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>


std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	std::cout << "Filename " << std::string(filename) << " not found \n";
	throw(errno);
	return NULL;
}


Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) : uKeeper(0)
{
	compile(vertexShaderFile, fragmentShaderFile);
}

Shader::Shader(std::string vertexShaderFile, std::string fragmentShaderFile) : uKeeper(0)
{
	compile(vertexShaderFile.c_str(), fragmentShaderFile.c_str());
}

Shader::Shader() : uKeeper(0) {};

void Shader::initialize(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	if (uKeeper.shaderID != 0)
	{
		std::cout << "Shader was already initialized..." << std::endl;
		return;
	}

	compile(vertexShaderFile, fragmentShaderFile);
}

void Shader::compile(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	/*
	  Shaders. They convert the crops into the bread, the basis of it all:
	  They take the vertices and convert it all to an image that can be
	  rendered to the screen.

	  To get a full Shader program, we have to proceed as fallows:
	    - Crea	std::cout<< "Done\n";
te shaders using the source code
		- Put the shaders into a program (a pipeline)
	*/
	
	std::string verSh = get_file_contents(vertexShaderFile);
	std::string frSh = get_file_contents(fragmentShaderFile);

	const char* vertexShaderSource = verSh.c_str();
	const char* fragmentShaderSource = frSh.c_str();

	// Create Vertex Shader, using one stream
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, SLProgramType::VertexShader);

	// Create Fragment Shader, using one stream
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, SLProgramType::FragmentShader);

	// Now we create the pipeline
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	compileErrors(ID, SLProgramType::Program);

	// Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	uKeeper.setShaderID(ID);
}

void Shader::registerUniform(std::string name)
{
	uKeeper.registerUniform(name);
}


void Shader::setUniformValueF(std::string name, float value)
{
	uKeeper.setUniformValueF(name, value);
}

void Shader::setUniformValue4F(std::string name, float* value)
{
	uKeeper.setUniformValue4F(name, value);
}

void Shader::setUniformValueI(std::string name, int value)
{
	uKeeper.setUniformValueI(name, value);
}

void Shader::setUniformValue4fv(std::string name, const float* value)
{
	uKeeper.setUniformValue4fv(name, value);
}

void Shader::compileErrors(unsigned int shader, SLProgramType type)
{
	int hasCompiled;
	const int MSGLEN = 256;
	char log[MSGLEN];

	if (type != SLProgramType::Program)
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, MSGLEN, NULL, log);
			std::cout << "SHADER COMPILATION ERROR for " << SLProgramTypeToString(type) <<
			 "\n" << log << "\n";

		}
	} 
	else 
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, MSGLEN, NULL, log);
			std::cout << "SHADER LINKING ERROR for " << SLProgramTypeToString(type) << "\n" << log << "\n";
		}
	}
}

std::string SLProgramTypeToString(SLProgramType s)
{
	switch(s)
	{
		case SLProgramType::Program: return "Program";
		case SLProgramType::VertexShader: return "VertexShader";
		case SLProgramType::FragmentShader: return "FragmentShader"; 
	}
	return "Unknown";
}

void Shader::activate()
{	
	glUseProgram(ID);
	//if (uKeeper != NULL) uKeeper.activate();
	uKeeper.passUniformValues();
}

void Shader::del()
{
	glDeleteProgram(ID);
}
