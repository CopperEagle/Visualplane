SHELL=/bin/bash


GRAPHICS2D=Shape.o Square.o DrawSquare.o Path.o ModifiableCenterShape.o TextLine.o TexturedColor.o Image.o
GRAPHICSCORE=Texture.o ImageAtlas.o Shader.o UniformKeeper.o VAO.o Camera.o Scene.o ShaderKeeper.o generate_gl_helper.o
POSTPROCEFFECTS=PostProcEffect.o FrozenCurveEffect.o WaterEffect.o
DATASTRUCTURES=MinHeap.o
UTIL=window.o FrameRateEnforcer.o record.o Spinner.o logging.o timing.o record_ffmpeg_log.o termcolor.o
DEPENDENCIES=glad.o stb_image.o# and also glm for camera

OBJS= $(DATASTRUCTURES) $(GRAPHICS2D) $(GRAPHICSCORE) $(POSTPROCEFFECTS) $(UTIL) $(DEPENDENCIES)
LIBS=-ldl -lglfw -lavcodec -lavformat -lavutil -lswscale -lfreetype -pthread
CFLAGS= -fPIC -O3 -Wpedantic -Wextra -I/usr/include/freetype2# -fsanitize=address -fsanitize=undefined -fstack-protector-all # -O3 -mavx2 -ftree-vectorize -ftree-loop-vectorize -ftree-parallelize-loops=2# -fdevirtualize -fsanitize=address -fsanitize=undefined -fstack-protector-all 


TOP=../src/
LOCATIONOUTPUT=../
EXECUTEABLE_NAME=libvisualplane.so

$(EXECUTEABLE_NAME):$(OBJS)
	g++ $(CFLAGS) -o $(LOCATIONOUTPUT)$(EXECUTEABLE_NAME) $(OBJS) $(LIBS) -shared 

window.o:$(TOP)window/window.cpp
	g++ $(CFLAGS) -c $(TOP)window/window.cpp

record_ffmpeg_log.o:$(TOP)window/record_ffmpeg_log.c
	gcc $(CFLAGS)  -c $(TOP)window/record_ffmpeg_log.c

record.o:$(TOP)window/record.cpp
	g++ $(CFLAGS)  -c $(TOP)window/record.cpp
	
Spinner.o:$(TOP)util/Spinner.cpp
	g++ $(CFLAGS) -c $(TOP)util/Spinner.cpp

logging.o:$(TOP)util/logging.cpp
	g++ $(CFLAGS) -c $(TOP)util/logging.cpp

timing.o:$(TOP)util/timing.cpp
	g++ $(CFLAGS) -c $(TOP)util/timing.cpp

termcolor.o:$(TOP)util/termcolor.cpp
	g++ $(CFLAGS) -c $(TOP)util/termcolor.cpp
	
FrameRateEnforcer.o:$(TOP)window/FrameRateEnforcer.cpp
	g++ $(CFLAGS) -c $(TOP)window/FrameRateEnforcer.cpp

glad.o:$(TOP)dependencies/glad.c
	g++ $(CFLAGS) -c $(TOP)dependencies/glad.c

Texture.o:$(TOP)graphics/core/textures/Texture.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/core/textures/Texture.cpp

ImageAtlas.o:$(TOP)graphics/core/textures/ImageAtlas.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/core/textures/ImageAtlas.cpp

Shader.o:$(TOP)graphics/core/shaders/Shader.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/core/shaders/Shader.cpp

UniformKeeper.o:$(TOP)graphics/core/shaders/UniformKeeper.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/core/shaders/UniformKeeper.cpp

VAO.o:$(TOP)graphics/core/bufferArrays/VAO.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/core/bufferArrays/VAO.cpp

stb_image.o:$(TOP)dependencies/stb/stb_image.cpp
	g++ $(CFLAGS) -msse2 -c $(TOP)dependencies/stb/stb_image.cpp
	
Shape.o:$(TOP)graphics/2D/Shape.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/2D/Shape.cpp
	
Square.o:$(TOP)graphics/2D/Square.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/2D/Square.cpp

Path.o:$(TOP)graphics/2D/Path.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/2D/Path.cpp

TextLine.o:$(TOP)graphics/2D/TextLine.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/2D/TextLine.cpp

Image.o:$(TOP)graphics/2D/Image.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/2D/Image.cpp

PostProcEffect.o:$(TOP)graphics/core/postproceffects/PostProcEffect.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/core/postproceffects/PostProcEffect.cpp

FrozenCurveEffect.o:$(TOP)graphics/core/postproceffects/FrozenCurveEffect.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/core/postproceffects/FrozenCurveEffect.cpp

WaterEffect.o:$(TOP)graphics/core/postproceffects/WaterEffect.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/core/postproceffects/WaterEffect.cpp

TexturedColor.o:$(TOP)graphics/core/color/TexturedColor.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/core/color/TexturedColor.cpp

ModifiableCenterShape.o:$(TOP)graphics/2D/ModifiableCenterShape.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/2D/ModifiableCenterShape.cpp
	
DrawSquare.o:$(TOP)graphics/2D/draw_backend/DrawSquare.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/2D/draw_backend/DrawSquare.cpp

MinHeap.o:$(TOP)datastructures/MinHeap.cpp
	g++ $(CFLAGS) -c $(TOP)datastructures/MinHeap.cpp

Scene.o:$(TOP)graphics/scene/Scene.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/scene/Scene.cpp

ShaderKeeper.o:$(TOP)graphics/scene/ShaderKeeper.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/scene/ShaderKeeper.cpp

generate_gl_helper.o:$(TOP)graphics/core/bufferArrays/generate_gl_helper.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/core/bufferArrays/generate_gl_helper.cpp

Camera.o:$(TOP)graphics/Camera.cpp
	g++ $(CFLAGS) -c $(TOP)graphics/Camera.cpp

