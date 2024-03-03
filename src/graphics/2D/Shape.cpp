#include "Shape.h"
#include "../../util/logging.h"
#include "../scene/Scene.h"
#include <glm/ext/matrix_transform.hpp>


float linear(float a, float b, float t)
{
	return a + (b-a)*t;
}

static float sigmoid(float x)
{
	return 1.0f/(1.0f + exp(-x));
}

float ease(float a, float b, float t)
{
	//return a + t*t*(3 - 2*t)*(b-a);
	float error = 0.00669285; //sigmoid(-10.0f / 2);
    float param = fmin(
        fmax((sigmoid(10.0f * (t - 0.5)) - error) / (1 - 2 * error), 0),
        1
    );
	return a + param*(b-a);
}

/*!
* @brief Constructor of shape
* Note that if z_layer is NO_SCREEN_RENDER (smallest int value possible) then
* it will not be rendered (not registered within the scene).
**/
Shape::Shape(Scene &s, int z_lay):model(1.0f), scene(s), z_layer(z_lay)
{
	if (z_lay != NO_SCREEN_RENDER) ID = scene.register_shape(*this);
}

/*!
* @brief Constructor of shape
* Note that if z_layer is NO_SCREEN_RENDER (smallest int value possible) then
* it will not be rendered (not registered within the scene).
**/
Shape::Shape(Scene &s, int x, int y, int z_lay):model(1.0f), scene(s), 
		anchor_x(x), anchor_y(y), z_layer(z_lay)
{
	if (z_lay != NO_SCREEN_RENDER) ID = scene.register_shape(*this);
	//model = glm::translate(model, glm::vec3(x, y, 0.0f));
}

void Shape::set_opacity(float alpha, n_frames start, n_frames end, InterpolationFunction f)
{
	OpacityData d = {0, alpha};
	AnimationInstruction* a = new AnimationInstruction{start, end, this, {0,0,0,0}, 
							(void*)f, SETOPACITY | FRESHEVENT};
	a->data.opac = d;
	scene.register_event(a);
}

/*!
* This stores a move instruction in the buffer. An idea could be to store
* these calls globally in a density based intervall tree. What happens if 
* there are overlapping moves is not defined.
*/
void Shape::move(int x, int y, n_frames start, n_frames end, InterpolationFunction f)
{
	MoveData d = {0, 0, x, y};
	AnimationInstruction* a = new AnimationInstruction{start, end, this, {0,0,0,0},
								(void*) f, MOVE | FRESHEVENT};
	a->data.move = d;
	scene.register_event(a);
}


void Shape::rotate(float phi, n_frames start, n_frames end, InterpolationFunction f)
{
	RotateData d{0, phi};
	AnimationInstruction* a = new AnimationInstruction{start, end, this, {0,0,0,0},
								(void*) f,  ROTATE | FRESHEVENT};
	a->data.rot = d;
	scene.register_event(a);
}

void Shape::scale(float alpha, n_frames start, n_frames end, InterpolationFunction f)
{
	ScaleData d{0, alpha};
	AnimationInstruction* a = new AnimationInstruction{start, end, this, {0,0,0,0},
								(void*) f, SCALE | FRESHEVENT};
	a->data.scal = d;
	scene.register_event(a);
}


void Shape::set_position(int x, int y)
{
	anchor_x = x;
	anchor_y = y;
}


void Shape::set_rot(float phi)
{
	rot = phi;
}

void Shape::set_scalef(float alpha)
{
	scalef = alpha;
}


const glm::mat4& Shape::get_matrix()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(anchor_x, anchor_y, 0.0f));
	model = glm::rotate(model, rot, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scalef));
	return model;
}