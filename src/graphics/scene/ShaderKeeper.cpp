#include "ShaderKeeper.h"
#include <glm/gtc/type_ptr.hpp>


void ShaderKeeper::initialize(const Config& config)
{
    textured_color_postprocessor.initialize(
        (config.shaders_dir + "framebuffer_alpha.vert").c_str(),
        (config.shaders_dir + "framebuffer_alpha.frag").c_str()
    );
    textured_color_postprocessor.registerUniform("camMat");
    textured_color_postprocessor.registerUniform("tex_color");
    textured_color_postprocessor.registerUniform("effects");
    textured_color_postprocessor.registerUniform("radius_boxblur");
    textured_color_postprocessor.registerUniform("flash_mul");


    textured_color_apply_color.initialize(
        (config.shaders_dir + "framebuffer_color.vert").c_str(),
        (config.shaders_dir + "framebuffer_color.frag").c_str()
    );
    textured_color_apply_color.registerUniform("camMat");
    textured_color_apply_color.registerUniform("color_points");
    textured_color_apply_color.registerUniform("numColors");
    textured_color_apply_color.registerUniform("width");
    textured_color_apply_color.registerUniform("height");
}

Shader& ShaderKeeper::get_textured_color_apply_color(
    const glm::mat4 &camMat, int color_pt_array_textureid, int max_colors, int width, int height
)
{
    textured_color_apply_color.setUniformValue4fv("camMat", glm::value_ptr(camMat));
    textured_color_apply_color.setUniformValueI("color_points", color_pt_array_textureid);
    textured_color_apply_color.setUniformValueI("numColors", max_colors);
    textured_color_apply_color.setUniformValueI("width", width);
    textured_color_apply_color.setUniformValueI("height", height);

    return textured_color_apply_color;
}


Shader& ShaderKeeper::get_textured_color_postprocessor(
    const glm::mat4 &camMat, int src_textureid, unsigned effects, unsigned radius_boxblur, float flash_mul)
{
    textured_color_postprocessor.setUniformValue4fv("camMat", glm::value_ptr(camMat));
    textured_color_postprocessor.setUniformValueI("tex_color", src_textureid);
    textured_color_postprocessor.setUniformValueI("effects", effects);
    textured_color_postprocessor.setUniformValueI("radius_boxblur", radius_boxblur);
    textured_color_postprocessor.setUniformValueF("flash_mul", flash_mul);

    return textured_color_postprocessor;
}