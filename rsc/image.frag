#version 330 core

in vec2 tex_coord;
in float _alpha;
out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, tex_coord);
    FragColor.a *= _alpha;
}
