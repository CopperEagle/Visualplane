#include "ModifiableCenterShape.h"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

ModifiableCenterShape::ModifiableCenterShape(Scene &s, int z_layer): Shape(s, z_layer){}

ModifiableCenterShape::ModifiableCenterShape(Scene &s, int x, int y, int z_layer): Shape(s, x, y, z_layer){}

const glm::mat4& ModifiableCenterShape::get_matrix()
{

    center_x += anchor_x;
    center_y += anchor_y;
    glm::mat4 ret_from_center = glm::translate(glm::mat4(1.0f), glm::vec3(center_x, center_y, 0.0f));

    glm::mat4 rot_scale = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(0.0f, 0.0f, 1.0f));
	rot_scale = glm::scale(rot_scale, glm::vec3(scalef));
    
    glm::mat4 go_center = glm::translate(glm::mat4(1.0f), glm::vec3(-center_x, -center_y, 0.0f));
    glm::mat4 mv = glm::translate(glm::mat4(1.0f), glm::vec3(anchor_x, anchor_y, 0.0f));
    center_x -= anchor_x;
    center_y -= anchor_y;

    model = ret_from_center * rot_scale * go_center * mv;
	return model;
}


void ModifiableCenterShape::set_center(float x, float y)
{
    center_x = x;
    center_y = y;
}