
#include "Texture.h"
#include "../../../dependencies/stb/stb_image.h"
#include "../../../util/logging.h"

int Texture::getWidth() const
{
	return widthImg;
}

int Texture::getHeight() const
{
	return heightImg;
}

/*!
* 
*/
Texture::Texture(const char *path) : p(path)
{
    loadImg();
}

/*!
* @brief Loads the Image and, upon success, registers the texture in OpenGL.
*
* @note This method should only be called once.
*/
void Texture::loadImg()
{
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(p, &widthImg, &heightImg, &numColCh, 0);
    if (data == nullptr)
    {
        logging::error("Texture::loadImg", "Image " + std::string(p) + " was not found.");
        return;
    }
    registerImg();

}

/*!
* @brief Registers the image in OpenGL. Will also enforce some basic settings.
*/
void Texture::registerImg()
{
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0);

    bindTexture();
    setProperties();
    if (numColCh == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else if (numColCh == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else {
        logging::error("Texture::registerImg", "Currently only supporting 3 or 4 color channels (rgb, rgba). Found " + 
            std::to_string(numColCh) + " in " + std::string(p));
        unbindTexture();
        stbi_image_free(data);
        return;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
    unbindTexture();

}

/*!
* @brief Sets the image to have filter GL_NEAREST,  and GL_REPEAT at the edge.
*
* \todo Maybe one day make these properties parameters rather than hardcoding them.*/
void Texture::setProperties() const
{   
    // ASSUMING THAT THE TEXTURE IS BOUND
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


/*!
* @brief Binds the texture. 
*
* Do this before you call draw on the VAO that contains the relevant data.
*/
void Texture::bindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}
