#ifndef FROZENCURVEEFFECT_H
#define FROZENCURVEEFFECT_H

#include "PostProcEffect.h"

/*!
* @brief This is just testing the PostProc framework for now.
**/
class FrozenCurveEffect : public PostProcEffect
{
public:
    FrozenCurveEffect(const Config& config);
    //~SampleEffect();

    GLuint render(GLuint input_color_buffer, uint32_t frame);

private:
    Shader shader;

};

#endif // FROZENCURVEEFFECT_H