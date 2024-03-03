#include "generate_gl_helper.h"

void generate_gl::generate_float32_array_as_1Dtexture(GLuint& id, unsigned len, const float* data)
{
    glGenTextures(1, &id);
    glEnable(GL_TEXTURE_1D);
    glBindTexture(GL_TEXTURE_1D, id);

    {    // reserve the memory are for the color buffer
        glTexImage1D(
            GL_TEXTURE_1D, 0, GL_R32F, len,  0, GL_RED, GL_FLOAT, data
        );
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // GL_CLAMP_TO_BORDER
        //glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }

    glBindTexture(GL_TEXTURE_1D, 0);
}


void generate_gl::generate_frame_buffer(GLuint &fb, GLuint &tex, unsigned width, unsigned height)
{
    glGenFramebuffers(1, &fb); // Create the buffer "name"

    // bind the buffer so we can work with it
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    {
        // Create attachment: the color buffer and bind it, so we can work with it
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        {    // reserve the memory are for the color buffer
            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr
            );
            // \todo consider making the filter a parameter, same for out of texture behaviour
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_CLAMP_TO_BORDER
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Attach color buffer to the frame buffer we just created above
            glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0
            );

        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // unbind the buffer so we can again draw to the screen as usual.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}