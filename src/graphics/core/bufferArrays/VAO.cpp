#include "VAO.h"
#include <cassert>
#include <iostream>

#define TYPE_EBO GL_ELEMENT_ARRAY_BUFFER
#define TYPE_VBO GL_ARRAY_BUFFER


/*!
* Builds a VBOLayout. Assumes that the VBO will contain one type of information.
* (E.g. position, color, etc) This means the color and the position of the 
* vertices are not interleaved.
*********************************************************************************/
VBOLayout buildCommonLayout(GLuint layoutID, GLuint numComponents)
{
    int stride = numComponents * sizeof(GL_FLOAT);
    return VBOLayout{layoutID, numComponents, GL_FLOAT, GL_FALSE, stride, (void*)0};
}


/*!
* Constructor. As argument, pass the expected number of VBOs that this VAO
* will hold. You cannot create more than this many VBOs for this VAO.
*********************************************************************************/
VAO::VAO(int nVBO)
{
    assert(nVBO > 0);
    glGenVertexArrays(1, &ID);
    maxVBO = nVBO;
    vboArray = new int[nVBO];
}

VAO::VAO(VAO&& other) : ID(other.ID), currVBO(other.currVBO), maxVBO(other.maxVBO), EBO_ID(other.EBO_ID)
{
    vboArray = other.vboArray;
    other.invalidate();
}

VAO& VAO::operator=(VAO&& other)
{
    ID = other.ID, currVBO = other.currVBO, maxVBO = other.maxVBO, EBO_ID = other.EBO_ID;
    vboArray = other.vboArray;
    other.invalidate();

    return *this;
}

/** Used in the move operations to ensure the dtor does not delete any resources. */
void VAO::invalidate()
{
    vboArray = NULL;
    EBO_ID = 0;
}

VAO::~VAO()
{
    if (vboArray == NULL && EBO_ID == 0) return;
    for (unsigned i = 0; i < currVBO; i++)
    {
        deleteVBO(vboArray[i]);
    }
    delete[] vboArray;
    deleteEBO(EBO_ID);
    glDeleteVertexArrays(1, &ID);
}

/*!
* Attach a new VBO. Note that in this implementation, a VBO can contain only
* only one type of information (position, color, ...). You cannot interleave the
* the information for the diffrent vertices...
* The VBOLayout declares the Interface to the shader, You can build a commonly
* used interface with buildCommonLayout. Usage satisfies 
* GL_(STREAM|STATIC|DYNAMIC)_(DRAW|READ|COPY). 
*********************************************************************************/
void VAO::attachNewVBO(GLfloat* data, int lengthData, const VBOLayout& itf, GLenum usage)
{
    if (data == NULL){ // data is null, hence this has no effect.
        std::cout << "data is null\n"; return;
    } else if (currVBO == maxVBO){
        std::cout << "Cannot accept more VBOs.\n"; return;
    }
    
    // bind VAO
    bind();

    // create VBO
    GLuint vbo = createVBO(data, lengthData, usage);
    // declare the interface to the shaders
    glVertexAttribPointer(itf.layoutID, itf.numComponents, itf.type, itf.normalized,
     itf.stride, itf.offset);
    // enable the VAO
	glEnableVertexAttribArray(itf.layoutID);
    
    // finalizing by unbinding
    // VAO first then VBO
    unbind();
    unbindVBO(vbo);    

    vboArray[currVBO++] = vbo;
}


/*!
* Edit a VBO. This needs the index of creation, ie you need to know in which order
* you inserted the VBOs. This is not ideal, however enough for the cases here. 
*********************************************************************************/
void VAO::editVBO(GLfloat* data, int lengthData, const VBOLayout& itf, GLenum usage, unsigned idxVBO)
{
    if (data == NULL){ // data is null, hence this has no effect.
        std::cout << "data is null\n"; return;
    } else if (idxVBO >= maxVBO){
        std::cout << "Edit VBO: invalid index.\n"; return;
    }
    
    // bind VAO
    bind();

    int vbo = vboArray[idxVBO];

    // edit VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind VBO
	// Fill in the data into the VBO
	glBufferData(GL_ARRAY_BUFFER, lengthData*sizeof(GLfloat), data, usage); // fill VBO

    // declare the interface to the shaders
    glVertexAttribPointer(itf.layoutID, itf.numComponents, itf.type, itf.normalized,
     itf.stride, itf.offset);
    // enable the VAO
	glEnableVertexAttribArray(itf.layoutID);
    
    // finalizing by unbinding
    // VAO first then VBO
    unbind();
    unbindVBO(vbo);    
}


/*!
* Attach an index buffer to the VAO. You can attach only one EBO to this
* VAO for now. Usage satisfies GL_(STREAM|STATIC|DYNAMIC)_(DRAW|READ|COPY). 
*********************************************************************************/
void VAO::attachEBO(GLuint* data, int lengthData, GLenum usage)
{
    if (data == NULL){ // data is null, hence this has no effect.
        std::cout << "data is null\n"; return;
    } else if (EBO_ID != 0){
        std::cout << "Cannot accept more EBOs.\n"; return;
    }
    
    // bind VAO
    bind();
    // create EBO
    EBO_ID = createEBO(data, lengthData, usage);
    
    // finalizing by unbinding
    // VAO first then EBO
    unbind();
    unbindEBO(EBO_ID);
}


/*!
* Edit the single EBO that this VAO can store.
*********************************************************************************/
void VAO::editEBO(GLuint* data, int lengthData, GLenum usage)
{
    if (data == NULL){ // data is null, hence this has no effect.
        std::cout << "data is null\n"; return;
    } else if (EBO_ID == 0){
        std::cout << "Cannot edit zero EBO.\n"; return;
    }
    
    // bind VAO
    bind();
    
    // edit EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID); // bind VBO
	// Fill in the vertex data into the VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lengthData*sizeof(GLuint), data, usage); // fill VBO

    
    // finalizing by unbinding
    // VAO first then EBO
    unbind();
    unbindEBO(EBO_ID);
}

/*!
* Draws the object that this VAO is representing.
* nIndices: Some points may be used multiple times.
*********************************************************************************/
void VAO::draw(Shader& shader, int nIndices)
{

    shader.activate();
    bind();
    if (EBO_ID == 0){
        glDrawArrays(GL_TRIANGLES, 0, nIndices);
    } else {
        glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
    }
    
    unbind(); // ? may be good for multi VAO draw?
}


/*!
* Draws the object that this VAO is representing.
* nIndices: Some points may be used multiple times.
* MAKE SURE to activate the shader BEFORE calling this 
* or use the other draw method.
*********************************************************************************/
void VAO::draw(int nIndices)
{

    bind();
    if (EBO_ID == 0){
        glDrawArrays(GL_TRIANGLES, 0, nIndices);
    } else {
        glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
    }
    
    unbind(); // ? may be good for multi VAO draw?
}


/********************************************************************************
* Private methods.
*********************************************************************************/




/**
* Private function. Binds the VAO. It is NOT used outside.
* This is to hide the underlying specifics of OpenGL such that no errors due to
* binding mismanagement happen.
*********************************************************************************/
void VAO::bind()
{
	glBindVertexArray(ID);
}

/**
* Private function. Binds the VAO. It is NOT used outside. 
*********************************************************************************/
void VAO::unbind()
{
	glBindVertexArray(0);
}


/**
* Private function, as it leaves object in an inconcistent state.
* Given the data as an array of Floats, the length of the array and the usage,
* it creates a VBO. Usage satisfies GL_(STREAM|STATIC|DYNAMIC)_(DRAW|READ|COPY). 
* After the function returns the ID of the new VBO, this particular
* VBO is still bound!
*********************************************************************************/
GLuint VAO::createVBO(GLfloat* data, int length, GLenum usage)
{   
    GLuint vbo;
    // create VBO: gen Bif, bind, glBuffer,  unbind
    glGenBuffers(1, &vbo); // create
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind VBO
	// Fill in the vertex data into the VBO
	glBufferData(GL_ARRAY_BUFFER, length*sizeof(GLfloat), data, usage); // fill VBO

    return vbo;
}

/**
* Private function, as it deals with VBOs. They are not directly accessible.
* Binds a VBO that was created before.
*********************************************************************************/
void VAO::bindVBO(GLuint vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

/**
* Private function, as it deals with VBOs. They are not directly accessible.
* Unbinds a VBO that was created before.
*********************************************************************************/
void VAO::unbindVBO(GLuint vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
* Private function, as it deals with VBOs. They are not directly accessible.
* Deletes the VBO.
*********************************************************************************/
void VAO::deleteVBO(GLuint vbo)
{
    glDeleteBuffers(1, &vbo);
}


/**
* Private function, as it leaves object in an inconcistent state.
* Given the data as an array of Floats, the length of the array and the usage,
* it creates a ebo. Usage satisfies GL_(STREAM|STATIC|DYNAMIC)_(DRAW|READ|COPY). 
* After the function returns the ID of the new VBO, this particular
* ebo is still bound!
*********************************************************************************/
GLuint VAO::createEBO(GLuint* data, int length, GLenum usage)
{   
    GLuint ebo;
    // create VBO: gen Bif, bind, glBuffer,  unbind
    glGenBuffers(1, &ebo); // create
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // bind VBO
	// Fill in the vertex data into the VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, length*sizeof(GLuint), data, usage); // fill VBO

    return ebo;
}



/**
* Private function, as it deals with EBOs. They are not directly accessible.
* Unbinds a EBO that was created before.
*********************************************************************************/
void VAO::unbindEBO(GLuint ebo)
{
    glBindBuffer(TYPE_EBO, 0);
}

/***
* Private function, as it deals with EBOs. They are not directly accessible.
* Deletes the EBO.
*********************************************************************************/
void VAO::deleteEBO(GLuint ebo)
{
    glDeleteBuffers(1, &ebo);
}