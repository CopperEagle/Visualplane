#ifndef MODIFIABLECENTERSHAPE_H
#define MODIFIABLECENTERSHAPE_H

#include "Shape.h"

class ModifiableCenterShape : public Shape 
{
public:
    ModifiableCenterShape(Scene &s, int z_layer = 0);
	ModifiableCenterShape(Scene &s, int x, int y, int z_layer = 0);

    void set_center(float x, float y);
protected:
    const glm::mat4& get_matrix();

    float center_x = 0.0f, center_y = 0.0f;
};

#endif // MODIFIABLECENTERSHAPE_H