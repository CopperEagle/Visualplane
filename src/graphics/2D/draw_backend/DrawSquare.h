#ifndef DRAWSQUARE_H
#define DRAWSQUARE_H

#include "../../core/bufferArrays/VAO.h"
#include "../../core/shaders/Shader.h"
#include "../../core/color/ColorObject.h"
#include "../../core/color/TexturedColor.h"
#include <glm/glm.hpp>

class Camera;

class DrawSquare
{
public:
	DrawSquare(const Config& config);
	~DrawSquare(){};

	void draw();
	void draw(const TexturedColor* tcol);
	void pass_data(GLfloat* data, size_t len);
	void set_matrix(const glm::mat4& mat); // call before the draw call

	void set_opacity(float alpha);
	void set_color(const TexturedColor* col);
	void set_color(const ColorObject& col);
	void set_border(const ColorObject& col, int border_width = 2);

protected:
	Shader shader;
	VAO vao;
	float rgba[4] = {0.0f, 0.0f, 0.0f, 0.0f};	
	float border[4] = {1.0f, 1.0f, 1.0f, 1.0f};
};


#endif // DRAWSQUARE_H