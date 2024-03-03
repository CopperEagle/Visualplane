#include "Camera.h"

#include "../util/logging.h"
#include "glm/gtc/type_ptr.hpp"


Camera::Camera(float screen_width, float screen_height) : 
    perspective(glm::ortho(0.0f, screen_width, 0.0f, screen_height))
{}


void Camera::set_screen_viewport(int x0, int y0, int x1, int y1)
{
    if (x0 <= x1 || y0 <= y1)
    {
        logging::error("Camera::set_screen_viewport", "Errornous input.");
    }
    
    logging::error("Camera::set_screen_viewport", "Currently not implemented.");
}