#ifndef TEXTUREDCOLOR_H
#define TEXTUREDCOLOR_H

#include "../../../dependencies/glad.h"
#include "../shaders/Shader.h"
#include "ColorObject.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
#include "../postproceffects/PostProcEffect.h"

class Shape;
class Scene;

/*!
* \todo Currently, handling TextureColor is a mess. Many nonconst Pointers everywhere.
* At the minimum, we can do nonconst_ptr(cls) a macro. Also we should look at the procedure for getting
* TextureColors live. Currently we are rerendering them when we recognize that we have moved along a frame.
* Maybe we should have the Scene looking after them?
* \todo We currently accept frames of type uint32_t, should be n_frames. Should look at where to
* store things like common math, definitions. Make sure that not too much stuff is stored in 
* places where you then have no choice than to include everything that sits in there.
*/

/**
* The idea is the fallowing: You can use this as a drawing board
* and draw shapes on it, or use the drawing method it will provide. 
* However, you can only use this as a color that you can input into other
* shapes. This means that these shapes need to support taking such a
* texture as color, which may not be true for every shape, hence it is not
* in the shape interface. The goal should be to make it multiusable, i.e. it 
* can be used by multiple shapes at the same time.
*
* First of all, it is a rectangle you render to. Now a way I do not want to go
* is to provide UV textures for the shape that takes this as color (texed shape).
* So rather than that, it will provide the fallowing to the texed shape (shader):
*
*   - the screen coordinates of its lower left corner (this is actually provided by the texed shape)
*   - its width and height
*   - itself as a sampler2D
*
* At the beginning, this will not support modification after initialisation phase (before 
* generate_video call.) Later, it may well support animations (also indefinie ones?)
* but it may need special treatment by the shader... it requires a redraw every frame.
* The animations may well be handled with the existing animation framework.
* That redraw would need to be after handling animations but before the texed shapes...
* 
* It may well be that blurring and other effects can be done using a similar interface...
*/
class TexturedColor
{
public:
    TexturedColor(unsigned width, unsigned height, Scene& s, bool live = false);
    ~TexturedColor();
    
    void draw(uint32_t frame);

    // THIS IS FOR THE USER
    void add_shape(Shape& s); // draw shape onto texture
    void freeze(uint32_t frame = 0);

    void add_color(float x, float y, const ColorObject& col);
    void start_color(unsigned n);
    void end_color();

    void add_effect_boxblur(unsigned radius = 7);
    void add_effect_invert_colors();
    void add_effect_greyscale();
    void add_effect_flash(float mul = 10.0f);


    // THIS IS FOR THE SHAPE THAT USES THIS AS A TEXTURE
    void prepare_texture_for_drawing(Shader& shader, int texture_unit_for_color_buffer) const; // draw texture onto shape
    void cleanup_texture_after_drawing() const;

    // Auxillary
    int get_width() const {return width;};
    int get_height() const {return height;};

    const glm::mat4& get_matrix() const;

    void append_postproc_effect(PostProcEffect& ppeff);
    

private:
    void post_process_texture() const;
    
    void bind() const;
    void unbind() const;

    

    // Shader alpha_shader;
    // Shader color_shader;
    glm::mat4 mat;
    std::vector<Shape*> shapes;
    GLfloat* data_color = nullptr;
    PostProcEffect* first_effect = nullptr;
    Scene& scene;
    uint32_t draw_timestamp = 1 << 31; // -> biggest 32bit int
    float flash_mul = 1.0f;
    int num_colors = 0;
    int max_colors = 0; // this is max color
    unsigned width, height;
    unsigned effects = 0;
    unsigned radius_boxblur = 0;
    GLuint main_fb_ID, main_fb_color_buffer_ID;
    GLuint postproc_fb_ID, postproc_fb_color_buffer_ID;
    GLuint color_array_ID;
    bool frozen = false;
    bool live = false;
    bool color_added = false;
};


#endif // TEXTUREDCOLOR_H