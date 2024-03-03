#include "TexturedColor.h"
#include "../bufferArrays/VAO.h"
#include "../bufferArrays/generate_gl_helper.h"
#include "../../../util/logging.h"
#include "../../2D/Shape.h"
#include "../../scene/Scene.h"
#include "ColorObject.h"
#include <cstddef>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/integer.hpp>

#ifndef __glibc_unlikely
#define __glibc_unlikely(x) x
#endif

/**
* \todo Color: Add the possibility where you can specify a background at the beginning
* and then add colorpoints that have a radius
* \todo Color: Give a direction and then add colorstops
* \todo Color: Colorpoints with no bg may also have relative size, see commented
* out code in framebuffer_color.frag
* 
* \todo This class does a lot of different things, maybe should split up?
**/

/**
* This is a developer note. You may see that the TexturedColor goes the strange route
* of actually rendering the contents to the texture and then render the redult onto a
* second framebuffer, the alpha framebuffer. The main problem that we have here is 
* with the alpha channel. We have several requirements:
*
*   - Obviously, the endresult should have the same alpha as if it had just been rendered once.
*   - The blending function when rendering onto the textured shape needs to be standard
*      such that further rendering operations in the shader of that shape are not altered.
*   - At the time when the texture is created (by rendering onto it) then blendung function
*      must not be altered either.
*
* However, the double rendering (first onto texture then shape) does do a reblend, altering
* the appearance for anything that has a opacity of neither 1 or 0. 
* The rerender undoes the effect of double rendering. As the texture 
* will be rendered many many frames and maybe even several shapes use it,
* doing this single effort rerendering to avoid having to do recomputation
* every frame seems justified.
*
* Also this opend the door for more postprocessing?
*/

/**
* A shape-user note: Treat this as a texture you can use for any region of your shape.
* All you need to do is:
* - cpp side:   -prepare your shader accordingly
*               -call prepare_drawing, vao.draw, cleanup
* - Shader:     -take the texture as sampler2D uniform
*               -optionally you can also pass the width and height of the texture 
*               (e.g. to render a correctly scaled image)
* Hence, common uniforms: texture, width, height, whether you use the texture at all
* You can configure which slot the texture uses in the prepare_drawing call.
* You can just take the value of the texture without any processing to make it look
* exactly as if there had been no inbetween rendering onto this texture here.
*/


/*
* Does not support updating the texture in case the shapes moved, etc.
*/
TexturedColor::TexturedColor(unsigned width, unsigned height, Scene& s, bool live) : width(width), height(height),
    mat(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height))),
    scene(s), live(live), first_effect(nullptr)
{
    generate_gl::generate_frame_buffer(main_fb_ID, main_fb_color_buffer_ID, width, height);
    generate_gl::generate_frame_buffer(postproc_fb_ID, postproc_fb_color_buffer_ID, width, height);
    
    if (live){
        s.register_effect(*this);
    }
}

static inline int to_texture_unit(int unit_slot_number);

/*!
* @brief Prepares for for the draw call that prints this texture onto a shape.
* 
* @param shader The shader of the shape that wants this texture being applied to it.
* @param[in] texture_unit_for_color_buffer Which texture unit should be used.*/
void TexturedColor::prepare_texture_for_drawing(Shader& shader, int texture_unit_for_color_buffer) const
{
    if (__glibc_unlikely(!frozen))
    {
        logging::error("TexturedColor::prepare_texture_for_drawing", "Must freeze() TexturedColor object after all shapes have been added.");
    }
    shader.activate();
    glActiveTexture(to_texture_unit(texture_unit_for_color_buffer));
    glBindTexture(GL_TEXTURE_2D, postproc_fb_color_buffer_ID);
}

/*!
* @brief Does some cleanup after the texture has been rendered onto a shape 
*/ 
void TexturedColor::cleanup_texture_after_drawing() const
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*!
* @brief Add a shape that will be drawn ONTO this texture.
**/
void TexturedColor::add_shape(Shape &s)
{
    if (live)
        shapes.push_back(&s);
    
    bind();
    glViewport(0, 0, width, height);
    s.draw(0, get_matrix());
    unbind();
    glViewport(0, 0, scene.get_config().width, scene.get_config().height);
}

void TexturedColor::draw(uint32_t frame)
{
    bind();
    glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
    for (Shape* shape: shapes)
    {
        if (shape == nullptr)
        {
            end_color();
            continue;
        }
        bind();
        glViewport(0, 0, width, height);
        glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

        shape->draw(frame, get_matrix());
        
        
    }
    unbind();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, scene.get_config().width, scene.get_config().height);
    freeze(frame);
}


const glm::mat4& TexturedColor::get_matrix() const
{
    return mat;
}


TexturedColor::~TexturedColor()
{
    if (data_color != nullptr) delete[] data_color;
    glDeleteTextures(1, &color_array_ID);
    glDeleteTextures(1, &postproc_fb_color_buffer_ID);
    glDeleteFramebuffers(1, &postproc_fb_ID);
    glDeleteTextures(1, &main_fb_color_buffer_ID);
    glDeleteFramebuffers(1, &main_fb_ID);
}


void TexturedColor::bind() const 
{
    glBindFramebuffer(GL_FRAMEBUFFER, main_fb_ID);
}

void TexturedColor::unbind() const 
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TexturedColor::freeze(uint32_t frame)
{
    post_process_texture();
    frozen = true;
    // now postproc_fb_color_buffer_ID contains the final result
    if (first_effect != nullptr) // if there are additional postprocessing effects registered
    {
        GLuint res_fb_buf = first_effect->render(postproc_fb_color_buffer_ID, frame);
        // now the ultimative final image (rendered potentially by many posproceff) is in res_buf
        // let us get it back onto our postproc_fb_color_buffer_ID by blitting it
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, res_fb_buf); // src
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postproc_fb_ID); // dst
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glViewport(0, 0, scene.get_config().width, scene.get_config().height);
}

void TexturedColor::start_color(unsigned n)
{
    if (data_color != nullptr)
    {
        logging::error("TexturedColor::start_color", "Already called, ignoring.");
        return;
    }
    else if (n == 0)
    {
        logging::error("TexturedColor::start_color", "Requesting zero colors. Either request some or remove call.");
        return;
    }
    data_color = new GLfloat[n*6];
    max_colors = n;
    num_colors = 0;
}

void TexturedColor::add_color(float x, float y, const ColorObject& col)
{
    if (num_colors == max_colors)
    {
        logging::error("TexturedColor::add_color", "Cannot add color: Maximum " + 
            std::to_string(max_colors) + " reached.");
        return;
    }
    data_color[2*num_colors] = x/width;
    data_color[2*num_colors + 1] = y/height;

    data_color[2*max_colors + 4*num_colors] = col.r;
    data_color[2*max_colors + 4*num_colors + 1] = col.g;
    data_color[2*max_colors + 4*num_colors + 2] = col.b;
    data_color[2*max_colors + 4*num_colors + 3] = col.a;
    
    num_colors++;
}

void TexturedColor::end_color()
{
    if (num_colors < max_colors)
    {
        logging::error("TexturedColor::end_color", "Cannot yet end color: Requested " + 
            std::to_string(max_colors) + " colors but provided " + std::to_string(num_colors) + ".");
        return;
    }
    if (!color_added) // this is called just once.
    {
        if (live) shapes.push_back(nullptr);
        generate_gl::generate_float32_array_as_1Dtexture(color_array_ID, 6*max_colors, data_color);
        color_added = true;
    }

    VAO vao(2);
    GLfloat rect[] = {0.0f, 0.0f,
        static_cast<GLfloat>(width), 0.0f,
        static_cast<GLfloat>(width), static_cast<GLfloat>(height),
        0.0f, static_cast<GLfloat>(height)};
    GLuint indices[] = {0, 1, 2, 0, 3, 2};
    
    vao.attachNewVBO(rect, 8, buildCommonLayout(0,2), GL_STATIC_DRAW);
    vao.attachEBO(indices, 6, GL_STATIC_DRAW);

    Shader& shader = scene.get_shaders().get_textured_color_apply_color(
        get_matrix(), 0, max_colors, width, height);

    
    shader.activate();
    bind();
    glViewport(0, 0, width, height);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, color_array_ID);
    glActiveTexture(GL_TEXTURE0);
    vao.draw(6);
    unbind();
    glViewport(0, 0, scene.get_config().width, scene.get_config().height);
    
}

void TexturedColor::post_process_texture() const
{    

    VAO vao(2);
    GLfloat rect[] = {0.0f, 0.0f,
        static_cast<GLfloat>(width), 0.0f,
        static_cast<GLfloat>(width), static_cast<GLfloat>(height),
        0.0f, static_cast<GLfloat>(height)};
    GLuint indices[] = {0, 1, 2, 0, 3, 2};
    GLfloat tex[] = {0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f};

    vao.attachNewVBO(rect, 8, buildCommonLayout(0,2), GL_STATIC_DRAW);
    vao.attachNewVBO(tex, 8, buildCommonLayout(1,2), GL_STATIC_DRAW);
    vao.attachEBO(indices, 6, GL_STATIC_DRAW);

    Shader& shader = scene.get_shaders().get_textured_color_postprocessor(
        get_matrix(), 0, effects, radius_boxblur, flash_mul);

    glBindFramebuffer(GL_FRAMEBUFFER, postproc_fb_ID);
    
    shader.activate();
    glBlendFunc(GL_ONE, GL_ZERO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, main_fb_color_buffer_ID);
    glViewport(0, 0, width, height);
    glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
    vao.draw(6);
    unbind();
    glViewport(0, 0, scene.get_config().width, scene.get_config().height);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static inline int to_texture_unit(int unit_slot_number)
{
    return GL_TEXTURE0 + unit_slot_number;
}

void TexturedColor::add_effect_boxblur(unsigned radius)
{
    radius_boxblur = radius;
    effects |= scene.get_shaders().get_textured_color_postprocessor_effectid_BOXBLUR();
}

void TexturedColor::add_effect_greyscale()
{
    effects |= scene.get_shaders().get_textured_color_postprocessor_effectid_GREYSCALE();
}

void TexturedColor::add_effect_invert_colors()
{
    effects |= scene.get_shaders().get_textured_color_postprocessor_effectid_INVERTCOLORS();
}

void TexturedColor::add_effect_flash(float mul)
{
    flash_mul = mul;
}

void TexturedColor::append_postproc_effect(PostProcEffect& ppeff)
{
    if (first_effect != nullptr)
    {
        first_effect->chain_effect(ppeff);
        return;
    }
    if (!ppeff.initialize(width, height))
    {
        logging::error("TexturedColor::append_postproc_effect", "Effect complained, will not add.");
        return;
    }
    first_effect = &ppeff;

}
