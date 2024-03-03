#ifndef TEXT_H
#define TEXT_H

#include <cstdint>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#include "Shape.h"
#include "../core/color/ColorObject.h"
#include "draw_backend/DrawSquare.h"




class TextLine : public Shape
{
public:
	TextLine(int x, int y, std::string msg, Scene& s, std::string font = "P052-Roman.otf", unsigned font_size = 24, int z_layer = 0); // negative width?
	~TextLine();

	//void initialize();
	void draw(n_frames frame, const glm::mat4& cam_mat);
	void set_color(const ColorObject& col){inner_color = col;};
	void set_outer_color(const ColorObject& col){outer_color = col;}
	void enable_italics();
	void enable_outline(int outline_size = 1.0f);
	//void set_border(const ColorObject& col, int border_width = 2);
    
private:
    void render_text(const char *text, float x, float y);
	void render_text_outlined(wchar_t ch, int size,
                    float outlineWidth, int &x, int &y);
    Shader shader;
    Scene& s;
	FT_Face face;
    VAO vao;
	ColorObject inner_color = GRAY(1.0f);
	ColorObject outer_color = GREEN(1.0f);
	std::string _msg;
	int outline_size = 1.0f;
	GLuint chr[2];
	unsigned font_size;
	uint8_t style = 0;
	bool successfully_initialized = false;
    
};

#endif // TEXT_H