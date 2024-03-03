#ifndef PATH_H
#define PATH_H

#include "Shape.h"
#include "../core/shaders/Shader.h"
#include <vector>
#include "../core/bufferArrays/VAO.h"
#include "ModifiableCenterShape.h"


class Path : public ModifiableCenterShape
{
public:
    Path(Scene& s, unsigned path_width = 10, int z_layer = 0);

    void add_point(float x, float y);
    void draw(n_frames frame, const glm::mat4& cam_mat);
    void close();
    void end();
    void set_color(const ColorObject& col);

private:
    Shader shader;
    VAO vao;
    Scene& s;
    std::vector<glm::vec2> p;
    float pt2[4] = {1.0f, 1.0f, 1.0f, 0.2f};
    glm::vec2 first, second;
    const unsigned width;
    unsigned _nVertices = 0;
    GLuint ID;
};

#endif // PATH_H