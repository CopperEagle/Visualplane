#ifndef POSTPROCEFFECT_H
#define POSTPROCEFFECT_H

#include <glm/glm.hpp>

#include "../shaders/Shader.h"
#include "../../../config.h"


class TexturedColor;

/*!
* @brief Abstract base class for post processing effects. These will always end up being atached to a TexturedColor. (else will not be rendered)
*
* PostProcEffects have their own framebuffer and take a
**/
class PostProcEffect
{
public:
    PostProcEffect(const Config& config);
    ~PostProcEffect();

    // this will be called by the texturedColor, secondary effects.
    /*!
    * @brief Takes a color buffer as input and renders on top. Returns the color buffer with the result.
    *
    * @note It is not guaranteed that the result will be on this effects color buffer,
    * This is because it will call the successor PostProcEffect to render as well, if needed.*/
    virtual GLuint render(GLuint input_color_buffer, uint32_t frame) = 0;

    
    
protected:
    /*!
    * This will be called before the effect is added into the rendering pipeline. (append_postproc)
    * It may not be called multiple times, enforcing
    * that a postprocessing effect cannot be used in multiple textured colors. 
    * Returns true if successful. Single initialize ensures that all effects have the same width and height
    * and only a single successor, if at all.*/
    bool initialize(unsigned width, unsigned height);

    /*!
    * @note The order in which they are passed matters. So the first one that is passed
    * in here will be the effect that will be rendered after this one. The one submitted
    * after that will be second to next, etc. Note that every PosProcEffect only stores
    * its successor. Hence the second effect submitted here will be passed on to the successor.
    * (and all after that as well).*/
    void chain_effect(PostProcEffect& next); // may be pass through to fallow on

    void bind_fb();
    void unbind_fb();

    glm::mat4 camMat;
    PostProcEffect* successor; // not owning
    const Config& config;
    unsigned fb_ID, color_buffer_ID;
    unsigned width, height;

    friend TexturedColor;

};

#endif // POSTPROCEFFECT_H