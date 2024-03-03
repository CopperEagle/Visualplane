#ifndef SHADERKEEPER_H
#define SHADERKEEPER_H

#include "../../config.h"
#include "../core/shaders/Shader.h"
#include <glm/glm.hpp>

/*!
* @brief Globally stores commonly used shaders, some defaults for their uniforms, etc.
**/
class ShaderKeeper
{
public:
    //ShaderKeeper();
    // ~ShaderKeeper(){};

    void initialize(const Config& config);

    Shader& get_textured_color_postprocessor(
        const glm::mat4 &camMat, int src_textureid, unsigned effects, unsigned radius_boxblur = 3.0f, float flash_mul = 1.0f
    );
    int get_textured_color_postprocessor_effectid_GREYSCALE(){return 1;};
    int get_textured_color_postprocessor_effectid_INVERTCOLORS(){return 2;};
    int get_textured_color_postprocessor_effectid_BOXBLUR(){return 4;};

    Shader& get_textured_color_apply_color(
        const glm::mat4 &camMat, int color_pt_array_textureid, int max_colors, int width, int height
    );
    

private:

    Shader textured_color_postprocessor;
    Shader textured_color_apply_color;

};

#endif // SHADERKEEPER_H