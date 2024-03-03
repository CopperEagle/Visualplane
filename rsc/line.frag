#version 330 core

out vec4 fragColor;
uniform vec4 col;

void main()
{
    fragColor = col; //vec4(1.0f,1.0f,1.0f,1.0f);
}
