#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 tex;

out vec2 position;

uniform mat4 camMat;

void main()
{
	gl_Position = camMat * vec4(aPos, 0.0f, 1.0f);
	position = tex;
}
