#ifndef SHAPE_H
#define SHAPE_H

#include <stdint.h>
//#include "draw_backend/DrawShape.h"
#include "../core/color/ColorObject.h"
#include <glm/glm.hpp>

typedef unsigned int ShapeID;
typedef uint32_t n_frames;

 // t between 0 and 1
float linear(float a, float b, float t);
float ease(float a, float b, float t);

typedef float (*InterpolationFunction)(float, float, float);

class Scene;
class ModifiableCenterShape;

constexpr int NO_SCREEN_RENDER = 1 << (sizeof(int)*8 - 1);


/*!
 * This is a developer note, not for the enduser...
 * 
 * @brief Abstract base class. A shape is the basic object of what you will finally
 * render onto the screen. Every Shape Subclass, like Square, has an associated draw-
 * backend. The Shape object keeps track of the basic properties and runtime variables
 * that may vary from frame to frame. The draw backend takes that data and renders the
 * object accordingly.
 * 
 * The rendering also has two different parts: The vertices data and the color (pattern), and
 * belonging to all this, the shader.
 * 
 * \todo Fix z changing: When changing to/from NO_SCREEN_RENDER, ensure desired sideeffect
 * \todo Move event handling from Scene to here.
 * \todo Enable color animation.
 * \todo Allow custom (shape-defined) event where handling is done by the shape itself.
 * 
 **/
class Shape
{

public:
	//Shape(){};
	Shape(Scene &s, int z_layer = 0);
	Shape(Scene &s, int x, int y, int z_layer = 0);

	ShapeID id(){return ID;};
	//virtual void animate(n_frames frame) = 0;
	virtual void draw(n_frames frame, const glm::mat4& cam_mat) = 0;
	virtual void set_color(const ColorObject& col) = 0;
	void set_opacity(float alpha){opacity = alpha;};

	// these are not used by the user directly.
	// this interface is used by the animation object.
	virtual void set_opacity(float alpha, n_frames start, n_frames end, InterpolationFunction f = linear);
	virtual void set_color(const ColorObject& s, n_frames start, n_frames end){};
	void move(int x, int y, n_frames start, n_frames end, InterpolationFunction f = linear);
	void scale(float alpha, n_frames start, n_frames end, InterpolationFunction f = linear);
	void rotate(float phi, n_frames start, n_frames end, InterpolationFunction f = linear);

	int get_z_layer(){return z_layer;};

	
protected:
	void set_position(int x, int y); // updates the anchor points for further use.
	void set_rot(float phi);
	void set_scalef(float alpha);
	int get_x(){return anchor_x;};
	int get_y(){return anchor_y;};
	float get_rot(){return rot;};
	float get_scalef(){return scalef;};
	float get_opacity(){return opacity;};
	virtual const glm::mat4& get_matrix();

private:
	glm::mat4 model;

	friend ModifiableCenterShape;

protected:
	Scene& scene;
	float opacity = 1.0f;
	float rot = 0.0f;
	float scalef = 1.0f;
	int anchor_x = 0, anchor_y = 0;
	int z_layer = 0;
	ShapeID ID = 0; // this is initialized by the Scene after registering

	friend Scene;
};


#endif // SHAPE_H