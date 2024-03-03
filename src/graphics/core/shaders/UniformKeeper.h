#ifndef UNIFORMKEEPER_H
#define UNIFORMKEEPER_H

#include <string>
#include <map>

/*!
* Keeps the uniforms for a particular shaderprogram. It allows to store the
* corresponding values at a single place until the shader is activated.
*
* The solution of lpacing uniforms into a map is inefficient.
*/
class UniformKeeper
{
    public:
        unsigned int shaderID;
        UniformKeeper(unsigned int shaderID);
        //~UniformKeeper();

        void setShaderID(unsigned int ID);
        void registerUniform(std::string name, bool reDefinitionWarning = true);
        void setUniformValueF(std::string name, float value);
        void setUniformValueI(std::string name, int value);
        void setUniformValue4F(std::string name, float* value);
        void setUniformValue4fv(std::string name, const float* matrix);

        void passUniformValues();

    private:
        std::map<std::string, unsigned int> uniformIDs;
		std::map<unsigned int, float> uValuesF;
        std::map<unsigned int, float*> uValues4F;
        std::map<unsigned int, int> uValuesI;
        std::map<unsigned int, const float*> uValuesFV;
};



#endif // UNIFORMKEEPER_H