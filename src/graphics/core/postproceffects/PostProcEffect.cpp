#include "PostProcEffect.h"
#include "../bufferArrays/generate_gl_helper.h"
#include "../../../util/logging.h"
#include <glm/ext/matrix_clip_space.hpp>


PostProcEffect::PostProcEffect(const Config& config): camMat(1.0f), width(0), height(0), successor(nullptr),
    fb_ID(0u), color_buffer_ID(0u), config(config)
{}

PostProcEffect::~PostProcEffect()
{
    if (color_buffer_ID == 0) return;
    glDeleteTextures(1, &color_buffer_ID);
    glDeleteFramebuffers(1, &fb_ID);
}

bool PostProcEffect::initialize(unsigned w, unsigned h)
{
    if (w == 0 || h == 0)
    {
        logging::error("PostProcEffect::initialize", "Will not accept width or height being zero.");
        return false;
    }
    if (width != 0)
    {
        logging::error("PostProcEffect::initialize", "Cannot reuse a PosProcEffect: You are either appending it to multiple TextureColors or reappending it to the same TexturedColor. Ignoring this reappending.");
        return false;
    }
    width = w;
    height = h;
    camMat = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    generate_gl::generate_frame_buffer(fb_ID, color_buffer_ID, width, height);
    return true;
}

void PostProcEffect::chain_effect(PostProcEffect& next)
{
    if (successor != nullptr)
    {
        successor->chain_effect(next);
        return;
    }
    
    if (!next.initialize(width, height))
    {
        logging::error("PostProcEffect::chain_effect", "Successor effect complained, will not add.");
        return;
    }
    successor = &next;
}

void PostProcEffect::bind_fb()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fb_ID);
}

void PostProcEffect::unbind_fb()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
/* render prototype:
GLuint PostProcEffect::render(GLuint input_color_buffer)
{
    shader.activate();
    bind();
    glViewport(0, 0, width, height);
    glActiveTexture(to_texture_unit(0));
    glBindTexture(GL_TEXTURE_2D, input_color_buffer);
    draw();
    unbind();
    return successor.render(color_buffer_ID);
    // viewport reset is on the caller, so the TexturedColor
}*/