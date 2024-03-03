#include "UniformKeeper.h"
#include <iostream>
#include "../../../dependencies/glad.h"


UniformKeeper::UniformKeeper(unsigned int ID)
{
    shaderID = ID;
}

/*!
* This should only be used by the shader class. It is needed because the ID
* is only known after the constructor has been executed.
*/
void UniformKeeper::setShaderID(unsigned int ID)
{
    shaderID = ID;
}


/*!
* Register a Uniform called <name>. If it has already been registered and 
* reDefinitionWarning is set, it will print a warning.
*/
void UniformKeeper::registerUniform(std::string name, bool reDefinitionWarning)
{
    auto uID = uniformIDs.find(name);
	if (uID == uniformIDs.end())
	{
		uniformIDs[name] = glGetUniformLocation(shaderID, name.c_str());
	} 
	else if (reDefinitionWarning)
	{
		std::cout << "WARNING: Trying to register the uniform " << name 
		<< " multiple times on the same shader.\n";
	}
	
}

/*!
* Set the value of a uniform. It must be previously registered on this
* particular shader to succeed. If this is not the case, there will be a warning.
*/
void UniformKeeper::setUniformValueF(std::string name, float value)
{
    auto uID = uniformIDs.find(name);
	if (uID == uniformIDs.end())
	{
		std::cout << "Uniform name " << name << " not found.\n";
	} 
	else 
	{
		uValuesF[uID->second] = value;
	}
}

void UniformKeeper::setUniformValue4F(std::string name, float* value)
{
    auto uID = uniformIDs.find(name);
	if (uID == uniformIDs.end())
	{
		std::cout << "Uniform name " << name << " not found.\n";
	} 
	else 
	{
		uValues4F[uID->second] = value;
	}
}

/*!
* Set the value of a uniform. It must be previously registered on this
* particular shader to succeed. If this is not the case, there will be a warning.
*/
void UniformKeeper::setUniformValueI(std::string name, int value)
{
    auto uID = uniformIDs.find(name);
	if (uID == uniformIDs.end())
	{
		std::cout << "Uniform name " << name << " not found.\n";
	} 
	else 
	{
		uValuesI[uID->second] = value;
	}
}


/*!
* Set the value of a uniform. It must be previously registered on this
* particular shader to succeed. If this is not the case, there will be a warning.
*/
void UniformKeeper::setUniformValue4fv(std::string name, const float* matrix)
{
    auto uID = uniformIDs.find(name);
	if (uID == uniformIDs.end())
	{
		std::cout << "Uniform name " << name << " not found.\n";
	} 
	else 
	{
		uValuesFV[uID->second] = matrix;
	}
}


/*!
* Passes the uniforms to the shader scripts. This is called after the shader was activated.
*/
void UniformKeeper::passUniformValues()
{
    for (auto& k: uValuesF)
	{
		glUniform1f(k.first, k.second);
	}

	for (auto& k: uValues4F)
	{
		glad_glUniform4f(k.first, k.second[0], k.second[1], k.second[2], k.second[3]);
	}

    for (auto& k: uValuesI)
	{
		glUniform1i(k.first, k.second);
	}

	for (auto& k: uValuesFV)
	{
		glUniformMatrix4fv(k.first, 1, GL_FALSE, k.second);
	}
}