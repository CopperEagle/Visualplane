#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(float screen_width, float screen_height);

    void set_screen_viewport(int x0, int y0, int x1, int y1);
    const glm::mat4& get_matrix() const {return perspective;};

private:
    glm::mat4 perspective;
};

#endif // CAMERA_H