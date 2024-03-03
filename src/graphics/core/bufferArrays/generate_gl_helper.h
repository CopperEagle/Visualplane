#ifndef GENERATE_GL_HELPER_H
#define GENERATE_GL_HELPER_H

#include "../../../dependencies/glad.h"

namespace generate_gl{

/*!
* @brief Generates a 1Dtexture of length len that can be used as 
* array of floats using texelFetch in the shader.
* 
* @param[out] id This will hold the id of the generated texture.
* @param[in] len The length of the array.
* @param[in] data The data of the array.
*
* @note This function only generates and initialises this new texture with the .
* provided data. It CANNOT be used to update this texture to change an item in this array.
*/
void generate_float32_array_as_1Dtexture(GLuint& id, unsigned len, const float* data);

/*!
* @brief Generates a framebuffer and attaches a color buffer (as texture) to it.
*
* @param[out] fb This will hold the id of the framebuffer after the function call.
* @param[out] tex This will hold the id of the color buffer that will be attached.
* @param[in] width The width of the color buffer.
* @param[in] height The height of the color buffer.
*
* @note It does not explicitly check whether all requirements are met. All it does is it
* generates a new framebuffer and attaches a color buffer of the given size.
**/
void generate_frame_buffer(GLuint &fb, GLuint &tex, unsigned width, unsigned height);
}

#endif // GENERATE_GL_HELPER_H