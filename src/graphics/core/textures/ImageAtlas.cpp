#include "ImageAtlas.h"
#include "../bufferArrays/VAO.h"
#include <string.h>
#include <iostream>


ImageAtlas::ImageAtlas(const char* path, int w, int h):
	img(path), v(NULL), nImgWidth(w), nImgHeight(h), nDraws(0){}


ImageAtlas::~ImageAtlas()
{
	if (v != NULL){ // if not first call
		delete[] v;
		delete[] tc;
		delete[] indices;
	}
}

void ImageAtlas::addDraw(int x, int y, int width, int height, unsigned idx)
{
	float baseX = (x - img.getWidth()) / float(img.getWidth());
	float baseY = (y - img.getHeight()) / float(img.getHeight());
	float widthX = width/float(img.getWidth());
	float widthY = height/float(img.getHeight());

	vertex.push_back(Float8{
		baseX + widthX, baseY + widthY,
		baseX + widthX, baseY,
		
		baseX, baseY,
		baseX, baseY + widthY
	});

	widthX = 1.0f / nImgWidth;
	widthY = 1.0f / nImgHeight;
	baseX = (idx % nImgWidth) * widthX;
	baseY = (nImgHeight - idx / nImgWidth) * widthY;

	texCoord.push_back( Float8{
		baseX + widthX, baseY + widthY,
		baseX + widthX, baseY,
		baseX, baseY,
		baseX, baseY + widthY
		
		
	});
	nDraws++;
}


void ImageAtlas::draw(Shader s)
{
	const int N_VERTEX = 8*vertex.size();
	const int N_TEX = 8*texCoord.size();
	const int N_IDX = 6*nDraws;

	if (changed)
		initData(N_VERTEX, N_TEX, N_IDX);
	//unsigned indices[] = { 0, 2, 1, 0, 3, 2,  4, 6, 5, 4, 7, 6 };//{ 0, 2, 1, 0, 3, 2 };
	
   
	VAO vao(2);


	vao.attachNewVBO(v, N_VERTEX, buildCommonLayout(0, 2), GL_STATIC_DRAW);
	vao.attachNewVBO(tc, N_TEX, buildCommonLayout(1, 2), GL_STATIC_DRAW);
	vao.attachEBO(indices, N_IDX, GL_STATIC_DRAW);

	s.setUniformValueI("tex0", 0);

	img.bindTexture();
	vao.draw(s, N_IDX);

}

void ImageAtlas::clear()
{
	vertex.clear();
	texCoord.clear();
	nDraws = 0;
}


void ImageAtlas::initData(int n_vert, int n_tex, int n_idx)
{
	if (v != NULL){ // if not first call
		delete[] v;
		delete[] tc;
		delete[] indices;
	}

	v = new float[n_vert];
	tc = new float[n_tex];
	indices = new unsigned[n_idx];
	
	float* vCnt = v;
	for (auto& elt: vertex){
		memcpy(vCnt, &elt, 8*sizeof(float));
		vCnt += 8;
	}
	vCnt = tc;
	for (auto& elt: texCoord){
		memcpy(vCnt, &elt, 8*sizeof(float));
		vCnt += 8;
	}
	
	for (int i = 0; i < nDraws; i++)
	{
		indices[6*i] = 0 + 4*i;
		indices[6*i + 1] = 2 + 4*i;
		indices[6*i + 2] = 1 + 4*i;
		indices[6*i + 3] = 0 + 4*i;
		indices[6*i + 4] = 3 + 4*i;
		indices[6*i + 5] = 2 + 4*i;
	}

	changed = false;
}


