#ifndef vao_h
#define vao_h

#include "../shaders/Shader.h"
#include "../textures/Texture.h"



struct VBOLayout
{
    GLuint      layoutID;       // unsigned integer, see the shader
    GLuint      numComponents;  // number of components this info has per vertex (eg pos: 3, etc.)
    GLenum      type;           // type of the data, GLFloat, usually
    GLboolean   normalized;     // usually GL_FALSE
    GLsizeiptr  stride;         // dist between first component of one and next entry, nElt * sizeof(type)
    void*       offset;         // usually (void*) 0
};

VBOLayout buildCommonLayout(GLuint layoutID, GLuint numComponents);


class VAO
{
    public:
        VAO(){}; // DO NOT USE THIS DIRECTLY
        VAO(int nVBO);
        VAO(VAO&& other);
        VAO& operator=(VAO&& other);
        ~VAO();

        void attachNewVBO(GLfloat* data, int lengthData, const VBOLayout& itf, GLenum usage);
        void attachEBO(GLuint* data, int lengthData, GLenum usage);
        void draw(Shader& shader, int nIndices);
        void draw(int nIndices);

        void editVBO(GLfloat* data, int lengthData, const VBOLayout& itf, GLenum usage, unsigned idxVBO);
        void editEBO(GLuint* data, int lengthData, GLenum usage);

        

    private:
        GLuint ID = 0;
        int* vboArray = NULL;
        unsigned currVBO = 0;
        unsigned maxVBO = 0;
        int EBO_ID = 0;

        void bind();
        void unbind();
        
        GLuint createVBO(GLfloat* data, int length, GLenum usage);
        void bindVBO(GLuint vbo);
        void unbindVBO(GLuint vbo);
        void deleteVBO(GLuint vbo);

        GLuint createEBO(GLuint* data, int length, GLenum usage);
        void unbindEBO(GLuint  ebo);
        void deleteEBO(GLuint ebo);


        void invalidate();
};


#endif 
