#include "WaterEffect.h"
#include "../bufferArrays/VAO.h"
#include "../../../util/logging.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../../config.h"

WaterEffect::WaterEffect(const Config& config) : PostProcEffect(config), 
    shader(config.shaders_dir + "postproc/postproc_watereffect.vert",
    config.shaders_dir + "postproc/postproc_watereffect.frag")
{
    shader.registerUniform("image");
    shader.registerUniform("time");
    shader.registerUniform("camMat");
}


GLuint WaterEffect::render(GLuint input_color_buffer, uint32_t frame)
{
    if (width == 0)
    {
        logging::error("FrozenCurveEffect::render", "Called render without initializing it.");
    }
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

    shader.setUniformValueI("image", 0);
    shader.setUniformValueF("time", float(frame)/config.fps);
    shader.setUniformValue4fv("camMat", glm::value_ptr(camMat));

    shader.activate();
    bind_fb();
    glViewport(0, 0, width, height);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, input_color_buffer);
    glBlendFunc(GL_ONE, GL_ZERO);
    vao.draw(6);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    unbind_fb();
    if (successor != nullptr)
        return successor->render(color_buffer_ID, frame);
    return fb_ID;
}