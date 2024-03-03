

# Visualplane 1.0

Visualplane is a library written in C++ that allows to create 2D animations using an easy-to-use interface. The output can be displayed and/or saved as a video. It is written from scratch using OpenGL, GLFW (for the window), FFmpeg (for the video) and the Freetype (for fonts).

This is an old project I wrote a few years ago in the span of two to three weeks.

## Features

- Rendering various Shapes, Text, Paths, Images
- Extensive color system including color gradients
- TexturedColor: Allowing for arbitrarily complex "colors" that are rendered scenes themselves
- Postprocessing effects (on TexturedColors only)
- Extensible Animation system
- Video and live generation


## Installation
The project was written and tested on Ubuntu 20.04 and retested on Ubuntu 22.04. It requires the following libraries

- FFmpeg libraries (Libavcodec 58.134.100, Libavformat 58.76.100, Libavutil 56.70.100, Libswscale 5.9.100)
- GLFW 3.3
- GLM 0.9.9.8
- Freetype 6.18.1

Once you have the prerequesites installed, open a terminal in this folder and perform

```bash
sh build.sh
```
This should produce a libvisualplane.so in the main directory. Then, if this is you first time using this library, I recommend checking out the examples:

```bash
cd examples/example_1
sh run.sh
```
This will compile the example code, link against the newly produced shared library and run it.

## Examples
There are a number of them in the examples directory.
![Example 1](examples/example_2/example_2.mp4)

![Example 2](examples/example_2/example_1.mp4)

## Limitations and Issues

- The FFmpeg libaries in use are deprecated at this point. I should update it.
- The basic framework is established. One could add more shapes, postprocessing effects, etc.
- Don't use make, use cmake...
- More documentation, examples.
- Various TODOs that are written down in the code.