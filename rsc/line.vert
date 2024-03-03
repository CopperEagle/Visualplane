#version 330 core

layout (location = 0) in vec2 aPos;


uniform mat4 camMat;
void main()
{
   gl_Position = camMat * vec4(aPos, 0.0f, 1.0f);
}