#include "Image.h"
#include "../scene/Scene.h"
#include "../core/bufferArrays/VAO.h"
#include "../../util/logging.h"
#include <glm/gtc/type_ptr.hpp>


Image::Image(const char* fpath, int x, int y, unsigned width, unsigned height, Scene& s, int z_layer) :
    Shape(s, x, y, z_layer), tex(fpath), shader(
        s.get_config().shaders_dir + "image.vert",
        s.get_config().shaders_dir + "image.frag"
    ), width(width), height(height)
{
    shader.registerUniform("camMat");
    shader.registerUniform("tex");
    shader.registerUniform("globalAlpha");
    shader.setUniformValueI("tex", 0);
}

void Image::set_color(const ColorObject &col)
{
    logging::error("Image::set_color", "Operation not supported.");
}

void Image::draw(n_frames frame, const glm::mat4 &cam_mat)
{
    float w = width/2.0f;
    float h = height/2.0f;

    VAO vao(2);
	GLfloat v[8] = {
		static_cast<GLfloat>(-w), static_cast<GLfloat>(-h),
		static_cast<GLfloat>(w),  static_cast<GLfloat>(-h),
		static_cast<GLfloat>(w),  static_cast<GLfloat>(h),
		static_cast<GLfloat>(-w), static_cast<GLfloat>(h)
	};
    GLuint indices[] = {0, 1, 2, 0, 3, 2};
    GLfloat texcoord[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f 
    };

    vao.attachNewVBO(v, 8, buildCommonLayout(0,2), GL_STATIC_DRAW);
    vao.attachNewVBO(texcoord, 8, buildCommonLayout(1,2), GL_STATIC_DRAW);
    vao.attachEBO(indices, 6, GL_STATIC_DRAW);

    const glm::mat4 mat = cam_mat * get_matrix();
    shader.setUniformValue4fv("camMat", glm::value_ptr(mat));
    shader.setUniformValueF("globalAlpha", opacity);

    shader.activate();
    glActiveTexture(GL_TEXTURE0);
    tex.bindTexture();

    vao.draw(6);


}