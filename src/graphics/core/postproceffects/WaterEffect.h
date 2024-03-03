#ifndef WATEREFFECT_H
#define WATEREFFECT_H

#include "PostProcEffect.h"

/*!
* @brief Water wave post processing effect.
**/
class WaterEffect : public PostProcEffect
{
public:
    WaterEffect(const Config& config);
    // ~WaterEffect();

    GLuint render(GLuint input_color_buffer, uint32_t frame);

private:
    Shader shader;

};

#endif // WATEREFFECT_H