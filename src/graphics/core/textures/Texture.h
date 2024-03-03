#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../../dependencies/glad.h"

/*!
* Texture class. Make sure that you bind the texture before you call draw
* on the VAO that contains the texturecoordinates for this texture for all
* the Vertices. An example is at the bottom of the file.
*/
class Texture 
{
    public:

        Texture(const char* path);
        ~Texture();
        int getWidth() const;
        int getHeight() const;

        void bindTexture() const;
        void unbindTexture() const;

    private:
        const char* p;
        int widthImg = 0, heightImg = 0, numColCh = 0;
        GLuint ID = 0;
        unsigned char* data;

        void loadImg();
        void registerImg();
        void setProperties() const;

};




#endif // TEXTURE_H

/* usage:
float square[] = {  -0.5f, -0.5f,
                        -0.5f, 0.5f,
                        0.5f, 0.5f,
                        0.5f, -0.5f };
GLuint indices2[] = {   0, 2, 1, 0, 3, 2 };

GLfloat textureCoordinates[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

Texture img("cute.png", true);
Shader shader("image.vert", "image.frag");
shader.registerUniform("tex0");

VAO vao(2);

vao.attachNewVBO(square, 8, buildCommonLayout(0, 2), GL_STATIC_DRAW);
vao.attachNewVBO(textureCoordinates, 8, buildCommonLayout(2, 2), GL_STATIC_DRAW);
vao.attachEBO(indices2, 6, GL_STATIC_DRAW);

shader.setUniformValueI("tex0", 0);

img.bindTexture();
vao.draw(shader, 9);

*/
