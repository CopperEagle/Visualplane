#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 texcoord;

out vec2 texpos;
uniform mat4 mat;

void main(void) {
  gl_Position = mat * vec4(aPos, 0, 1);
  texpos = texcoord;
}