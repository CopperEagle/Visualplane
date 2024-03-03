
#include "Shape.h"
#include "TextLine.h"
#include "../scene/Scene.h"
#include "../../util/os_path.h"
#include "../../util/logging.h"
//#include "freetype/ftglyph.h"
//#include "freetype/ftimage.h"

#include <cstring>


#include "../../dependencies/glad.h"

#include <glm/gtc/type_ptr.hpp>

constexpr float italics_angle = 3.14f/2.0f * (7.0f/9.0f);
const glm::mat4 italics_sheer = glm::mat4(glm::mat2(glm::vec2(1.0f, 0.0f), glm::vec2(glm::cos(italics_angle), glm::sin(italics_angle))));


FT_Library ft;
bool _ft_initiated = false;


#define CHARACTER_INSIDE 0
#define CHARACTER_OUTSIDE 1
#define STYLE_ITALICS 1u
#define STYLE_BOLD NOTIMPLEMENTED
#define STYLE_OUTLINE 4u

// \todo proper implementing color passing and choosing whether to have outline or not... deprecate enable_outline
// \todo Consider supporting double outline.
// \todo Provide functions that measure string length (Can read it off when doing rendering...)
// \todo split up into the part that generates the alpha map and the part that takes these and passes them to openGL.
// \todo enable opacity animations
// \todo Enable setting center, maybe alignment (center, left, right)


TextLine::TextLine(int x, int y, std::string str, Scene& s, std::string font, unsigned font_size, int z_layer) : 
	Shape(s, x, y, z_layer),
    shader(s.get_config().shaders_dir + "text.vert", 
    s.get_config().shaders_dir + "text.frag"),
    s(s), vao(2), _msg(str), font_size(font_size)
{
	if (!_ft_initiated){
		//logging::debug("TextLine::TextLine", "Currently only a single textline object is supported. Check todos.");
		if (FT_Init_FreeType(&ft)) {
			logging::error("TextLine::TextLine", "Could not initiate the freetype library");
			return;
		}
		_ft_initiated = true;
	}
	

	/* Load a font */
	std::string fontpath = join_paths(s.get_config().fonts_dir, font);
	if (FT_New_Face(ft, fontpath.c_str(), 0, &face)) {
		logging::error("TextLine::TextLine", "Could not open font " + font + " at " + fontpath);
		return;
	}

    shader.registerUniform("mat");
    shader.registerUniform("inner_chr_tx");
	shader.registerUniform("outer_chr_tx");
    shader.registerUniform("inner_color");
	shader.registerUniform("outer_color");
	shader.registerUniform("nTextures");

    shader.setUniformValueI("inner_chr_tx", 0);
	shader.setUniformValueI("outer_chr_tx", 1);
    shader.setUniformValue4F("inner_color", (float*)&inner_color);
	shader.setUniformValue4F("outer_color", (float*)&outer_color);
    
    float square[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    GLuint indices2[6] = { 0, 2, 1, 0, 3, 2 };

    GLfloat textureCoordinates[8] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };
    // pos
    vao.attachNewVBO(square, 8, buildCommonLayout(0, 2), GL_STATIC_DRAW);
    // tex coord
    vao.attachNewVBO(textureCoordinates, 8, buildCommonLayout(1, 2), GL_STATIC_DRAW);

    vao.attachEBO(indices2, 6, GL_STATIC_DRAW);

	// Create textures that will be used to hold one "glyph", one character
	for (int i = 0; i < 2; i++)
	{
		glGenTextures(1, &chr[i]);

		// bind this texture so we can work with it.
		glBindTexture(GL_TEXTURE_2D, chr[i]);

		// We require 1 byte alignment when uploading texture data
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Clamping to edges is important to prevent artifacts when scaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Linear filtering usually looks best for text
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	successfully_initialized = true;
}

TextLine::~TextLine()
{
	glDeleteTextures(2, chr);
	if (_ft_initiated){
		FT_Done_FreeType(ft);
		_ft_initiated = false;
	}
	
}

void TextLine::enable_italics()
{
	style = style | STYLE_ITALICS;
}

void TextLine::enable_outline(int outline_width)
{
	style = style | STYLE_OUTLINE;
	outline_size = outline_width;
}


void TextLine::render_text(const char *text, float x, float y) {
	
	shader.setUniformValueI("nTextures", 1);
	const char *p;
	FT_GlyphSlot g = face->glyph;
	
	// Activate texture that will hold the bitmap of the character
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, chr[CHARACTER_INSIDE]);

	// Loop through all characters
	for (p = text; *p; p++) {

		// Try to load and render the character
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))	
            continue;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		// Calculate the vertex and texture coordinates
		float w = g->bitmap.width;
		float h = g->bitmap.rows;
		float x2 = x + g->bitmap_left;
		float y2 = y + g->bitmap_top;

		GLfloat box[8] = {
			x2, y2 - h, 
			x2 + w, y2 - h, 
			x2 + w, y2,
			x2, y2
		};
		// Update position of the drawing box
        vao.editVBO(box, 8, buildCommonLayout(0, 2), GL_STATIC_DRAW, 0);
		// Draw the character on the screen
		vao.draw(shader, 6);

		// Advance the cursor to the start of the next character
		x += (g->advance.x >> 6);
		y += (g->advance.y >> 6);
	}

}


void TextLine::draw(n_frames frame, const glm::mat4& cam_mat)
{
    //shader.activate();
	if (!successfully_initialized) // avoid barrage of error messages.
		return;
	
    glm::mat4 mat = cam_mat * get_matrix();
	if (style & STYLE_ITALICS) mat =  mat * italics_sheer;

	shader.setUniformValue4fv("mat", glm::value_ptr(mat));
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FT_Set_Pixel_Sizes(face, 0, 1.2*font_size);
	if (style & STYLE_OUTLINE)
	{
		int x = 0;
		int y = 0;
		const char * msg = _msg.c_str();
		
		for (const char* pst = &msg[0]; *pst; pst++)
		{
			render_text_outlined(
					(long)*pst,
					1.2*font_size,
					outline_size,
					x, y);
		}	
	}
	else 
	{
		render_text(_msg.c_str(), 0, 0);
	}
}





// Scale src up into dst by padding with 0s
static void upscale(char* src, int sx, int sy, char* dst, int x, int y)
{
	if (sx > x || sy > y){
		logging::error("<static> upscale @ Textline.cpp:" + std::to_string(__LINE__), "Cannot upscale inner rendering of character.");
		return;
	}
	if (sx == x && sy == y)
	{
		memcpy(dst, src, sx*sy);
		return;
	}
	memset(dst, 0, x*y);
	int addleft = (x - sx)/2;
	int addtop = (y - sy + 1)/2;

	int i = 0;
	for (int h = addtop; h < addtop + sy; h++)
	{
		memcpy(dst + (addleft + h*x), src + i*sx, sx);
		i++;
	}

}


void set_font_size(FT_Face &face, int &size) {

	if (FT_Set_Char_Size(face, size << 6, size << 6, 90, 90)) {
		logging::error("TextLine::set_font_size", "Setting size of the character has failed.");
	}
}


void TextLine::render_text_outlined(wchar_t ch, int size, float outlineWidth, int &x, int &y) 
{
	
	shader.setUniformValueI("nTextures", 2);
	// Set the size to use.
	if (successfully_initialized) //avoid barrage of error messages
		set_font_size(face, size);

	// Load the glyph we are looking for.
	FT_UInt gindex = FT_Get_Char_Index(face, ch);
	if (FT_Load_Glyph(face, gindex, FT_LOAD_DEFAULT)) { // FT_LOAD_NO_BITMAP
		logging::error("TextLine::render_text_outlined", "Loading glyph for character " + std::to_string(ch) + "failed");
		return;
	}

    // Need an outline for this to work.
    if (face->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
		logging::error("TextLine::render_text_outlined", "Glyth not format outline");
		return;
	}

	// If whitespace, we can skip all the rendering
	// \todo this may actually be a problem for exotic fonts that have a diffrent symbol at his position
	if (ch == (long)' ')
	{
		x += (face->glyph->advance.x >> 6);
		y += (face->glyph->advance.y >> 6);
		return;
	}

	// The border is rendered first.
    // Set up a stroker.
    FT_Stroker stroker;
    FT_Stroker_New(ft, &stroker);
    FT_Stroker_Set(stroker, (int)(outlineWidth * 64),
                     FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

    FT_Glyph glyph;
    if (FT_Get_Glyph(face->glyph, &glyph)) {
		logging::error("TextLine::render_text_outlined", "Could not extract glyth for the given character.");
		return;	
	}

    FT_Glyph_StrokeBorder(&glyph, stroker, false, true);
    
	// Again, this needs to be an outline to work.
    if (glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
      logging::error("TextLine::render_text_outlined", "Cannot create an outline without glyph supporting such.");
	  return;
	}
	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);

	FT_BitmapGlyph outline_glyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
	int imgWidth = outline_glyph->bitmap.width, imgHeight = outline_glyph->bitmap.rows,
        imgSize = imgWidth * imgHeight; // Assuming here that the border is always bigger than the inner rendering

	char *inner = new char[2 * imgSize]; // \todo Avoid reallocating for every single character...
    char *outer = inner + imgSize;
	
	memcpy(outer, outline_glyph->bitmap.buffer, imgSize);
    
	// Next we render the inner part of the character
	if (FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL ))
	{
		logging::error("TextLine::render_text_outlined", "Error trying to render the inside of the character \"" + std::to_string(ch) + "\"");
	}
    // The alpha images should have same size. Assuming that the inner part is smaller
	upscale((char*)face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows,
		inner, imgWidth, imgHeight);

	// Clean up afterwards.
    FT_Stroker_Done(stroker);
    FT_Done_Glyph(glyph);
	

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, chr[CHARACTER_OUTSIDE]);
    glActiveTexture(GL_TEXTURE1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0,
                 GL_RED, GL_UNSIGNED_BYTE, outer);
    glBindTexture(GL_TEXTURE_2D, 0);
	
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, chr[CHARACTER_INSIDE]);
    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0,
                 GL_RED, GL_UNSIGNED_BYTE, inner);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Calculate the vertex coordinates
    float w = imgWidth;
    float h = imgHeight;
    float x2 = x + face->glyph->bitmap_left;
    float y2 = y + face->glyph->bitmap_top + outlineWidth;

    GLfloat box[8] = {x2, y2 - h, x2 + w, y2 - h, x2 + w, y2, x2, y2};
    // Update position of the drawing box
    vao.editVBO(box, 8, buildCommonLayout(0, 2), GL_STATIC_DRAW, 0);
    // Draw the character on the screen
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, chr[CHARACTER_INSIDE]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, chr[CHARACTER_OUTSIDE]);
    vao.draw(shader, 6);
	x += (face->glyph->advance.x >> 6) + outlineWidth;
	y += (face->glyph->advance.y >> 6);


    delete[] inner;    
}






/*

void Text::render_text(const char *text, float x, float y) {
	// float angle = 3.14/2;
	// matrix.xx = 0x10000L;
	// matrix.xy = (FT_Fixed)(cos( angle ) * 0x10000L );
	// matrix.yx = 0;
	// matrix.yy = (FT_Fixed)( sin( angle ) * 0x10000L );
	//FT_Set_Transform( face, &matrix, nullptr );

	const char *p;
	FT_GlyphSlot g = face->glyph;

	// Create a texture that will be used to hold one "glyph" 
	GLuint tex;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// We require 1 byte alignment when uploading texture data 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Clamping to edges is important to prevent artifacts when scaling 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Linear filtering usually looks best for text
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Loop through all characters
	for (p = text; *p; p++) {
		// Try to load and render the character
		//826
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER)){
			
            continue;
        }

		glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		// Calculate the vertex and texture coordinates 
		float w = g->bitmap.width;
		float h = g->bitmap.rows;
		float x2 = x + g->bitmap_left;
		float y2 = y + g->bitmap_top;

		GLfloat box[8] = {
			x2, y2 - h, 
			x2 + w, y2 - h, 
			x2 + w, y2,
			x2, y2
		};
        vao.editVBO(box, 8, buildCommonLayout(0, 2), GL_STATIC_DRAW, 0);
		// Draw the character on the screen
        
		vao.draw(shader, 6);

		// Advance the cursor to the start of the next character
		x += (g->advance.x >> 6);
		y += (g->advance.y >> 6);
	}

	glDeleteTextures(1, &tex);
}
*/
