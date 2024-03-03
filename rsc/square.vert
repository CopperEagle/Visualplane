#version 330 core
layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec3 aColor;

out vec2 relative_position;
//out vec3 color;

uniform mat4 camMat;

void main()
{
	gl_Position = camMat * vec4(aPos, 0.0f, 1.0f);
	relative_position = aPos;
	//color = aColor;
}
