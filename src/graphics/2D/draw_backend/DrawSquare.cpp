#include <iostream>
#include "DrawSquare.h"
#include "../../../config.h"
#include "../../Camera.h"

//#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



DrawSquare::DrawSquare(const Config& config): 
	vao(1),
	shader(
		config.shaders_dir + "square.vert", 
		config.shaders_dir + "square.frag"
		)
{
	shader.registerUniform("globalAlpha");
	shader.registerUniform("camMat");
	shader.registerUniform("colorValue");
	shader.registerUniform("colorBorderValue");
	shader.registerUniform("width");
	shader.registerUniform("borderWidth");
	shader.registerUniform("use_texture_color");

	shader.setUniformValue4F("colorBorderValue", border);
	shader.setUniformValueI("borderWidth", 2);
	shader.setUniformValue4F("colorValue", rgba);
	shader.setUniformValueI("use_texture_color", 0);
}

void DrawSquare::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	vao.draw(shader, 6);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void DrawSquare::draw(const TexturedColor* tcol)
{
	tcol->prepare_texture_for_drawing(shader, 0);
	vao.draw(6);
	tcol->cleanup_texture_after_drawing();
}

void DrawSquare::set_opacity(float alpha)
{
	shader.setUniformValueF("globalAlpha", alpha);
}


void DrawSquare::pass_data(GLfloat* data, size_t len)
{
	GLuint idx[6] = {0, 2, 1, 0, 3, 2};
	shader.setUniformValueI("width", -data[0]*2);
	
	
	// if (!initialized)
	// {		
	// here you use the VAO, you create stuff like the ebo etc.
	// now this will actually be placed inside the GraphicsObject? For custom shaders?....no itf should be same
	vao.attachNewVBO(data, 8, buildCommonLayout(0, 2), GL_STATIC_DRAW); // vertex data
	//vao.attachNewVBO(&col[0], 3, buildCommonLayout(1, 3), GL_STATIC_DRAW); // color data
	vao.attachEBO(&idx[0], 6, GL_STATIC_DRAW);
	//initialized = true;
	// } 
	// else 
	// {
	// 	vao.editVBO(data, 8, buildCommonLayout(0, 2), GL_STATIC_DRAW, 0); // vertex data
	// 	vao.editVBO(&col[0], 3, buildCommonLayout(1, 3), GL_STATIC_DRAW, 1); // color data
	// 	//vao.editEBO(&idx[0], 6, GL_STATIC_DRAW);
	// }
	
	
}

void DrawSquare::set_matrix(const glm::mat4 &mat)
{
	shader.setUniformValue4fv("camMat", glm::value_ptr(mat));
}


void DrawSquare::set_color(const ColorObject& col)
{
	rgba[0] = col.r;
    rgba[1] = col.g;
    rgba[2] = col.b;
	rgba[3] = col.a;
	shader.setUniformValue4F("colorValue", rgba);
	if (col.a < 0) return;
	set_opacity(col.a);
}

void DrawSquare::set_color(const TexturedColor* col)
{

	shader.registerUniform("tex_color");
	shader.registerUniform("tex_width");
	shader.registerUniform("tex_height");

	shader.setUniformValueI("use_texture_color", 1);
	shader.setUniformValueI("tex_color", 0);
	shader.setUniformValueI("tex_width", col->get_width());
	shader.setUniformValueI("tex_height", col->get_height());
}

void DrawSquare::set_border(const ColorObject& col, int border_width)
{
	border[0] = col.r;
    border[1] = col.g;
    border[2] = col.b;
	border[3] = col.a;
	shader.setUniformValue4F("colorBorderValue", border);
	shader.setUniformValueI("borderWidth", border_width);
}