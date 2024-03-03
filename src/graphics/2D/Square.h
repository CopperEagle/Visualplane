#ifndef SQUARE_H
#define SQUARE_H

#include <cstdint>

#include "Shape.h"
#include "../core/color/ColorObject.h"
#include "draw_backend/DrawSquare.h"

#include "../core/color/TexturedColor.h"

// Design choice: If you use ColoredTexture, it will start at the left bottom, taking the border into account ( see - borderWidth in fragmentshader)

//
class Square : public Shape
{
public:
	Square(int x, int y, int width, Scene& s, int z_layer = 0); // negative width?
	Square(int x, int y, int width, Scene& s, TexturedColor& tex, int z_layer = 0);
	~Square(){};

	void initialize();
	void draw(n_frames frame, const glm::mat4& cam_mat);

	void set_color(const ColorObject& col);
	void set_color(const TexturedColor& col);
	void set_border(const ColorObject& col, int border_width = 2);

private:
	DrawSquare s;
	const TexturedColor* tcol = nullptr;
	int width;
	
};

#endif // SQUARE_H