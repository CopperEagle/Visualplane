

#include <glm/ext/matrix_clip_space.hpp>

#include "Square.h"
#include "../scene/Scene.h"
#include "../../dependencies/glad.h"
#include "../core/color/TexturedColor.h"


Square::Square(int _x, int _y, int _width, Scene& s, int z_lay) : 
	Shape(s, _x, _y, z_lay), width(_width),
	s(s.get_config())
{

	initialize();
}


void Square::initialize(){

	float w = width/2.0f; // width

	GLfloat v[8] = {
		static_cast<GLfloat>(-w), static_cast<GLfloat>(-w),
		static_cast<GLfloat>(w), static_cast<GLfloat>(-w),
		static_cast<GLfloat>(w), static_cast<GLfloat>(w),
		static_cast<GLfloat>(-w), static_cast<GLfloat>(w)
	};

	s.pass_data(&v[0], 8);
}


void Square::draw(n_frames time, const glm::mat4& cam_mat)
{
	s.set_opacity(opacity);
	const glm::mat4 mat = cam_mat * get_matrix();
	s.set_matrix(mat);
	if (tcol != nullptr){
		s.draw(tcol); // draw using tcol as texture into another texture...
	} else {		
		s.draw();
	}
}


void Square::set_color(const ColorObject &col)
{
	s.set_color(col);
	// if (col.a < 0) return;
	// opacity = col.a;
}

void Square::set_color(const TexturedColor &col)
{
	tcol = &col;
	s.set_color(tcol);
}

void Square::set_border(const ColorObject& col, int border_width)
{
	s.set_border(col, border_width);
}