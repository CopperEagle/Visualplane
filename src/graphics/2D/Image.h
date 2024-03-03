#ifndef IMAGE_H
#define IMAGE_H

#include "Shape.h"
#include "../core/shaders/Shader.h"
#include "../core/textures/Texture.h"

/*!
* @brief Image rectangle that can be treated like any other shape.
**/
class Image : public Shape
{
public:
    Image(const char* fpath, int x, int y, unsigned width, unsigned height, Scene& s, int z_layer = 0);
    ~Image(){};

    void draw(n_frames frame, const glm::mat4& cam_mat);
    void set_color(const ColorObject& col);
    

private:
    Shader shader;
    Texture tex;
    unsigned width, height;

};

#endif // IMAGE_H