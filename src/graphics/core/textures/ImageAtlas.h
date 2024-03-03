#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include "../shaders/Shader.h"
#include "Texture.h"

struct Float8{
	float x1, y1, x2, y2, x3, y3, x4, y4;
};

class ImageAtlas {

public:
	ImageAtlas(const char* path, int w, int h);
	~ImageAtlas();

	void addDraw(int x, int y, int width, int height, unsigned img);
	void clear();
	void draw(Shader s);

private:
	void initData(int n_vert, int n_tex, int n_idx);

	const Texture img;
	std::vector<Float8> vertex, texCoord;
	float* v, *tc;
	unsigned* indices;
	int nImgWidth, nImgHeight, nDraws;
	bool changed = true;

};



#endif