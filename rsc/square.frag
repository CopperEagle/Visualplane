#version 330 core

in vec2 relative_position;
//in vec3 color;
out vec4 FragColor;

uniform float globalAlpha;
uniform vec4 colorValue;
uniform vec4 colorBorderValue;
uniform int width;
uniform int borderWidth;

// TexturedColor
uniform sampler2D tex_color;
uniform int use_texture_color;
uniform int tex_width;
uniform int tex_height;


void handle_border();
void handle_interior();

void main()
{	
	vec2 position = relative_position;
	if (position.x >= width/2 - borderWidth || position.x <= -width/2 + borderWidth){
		handle_border();
		return;
	} else if (position.y >= width/2 - borderWidth || position.y <= -width/2 + borderWidth){
		handle_border();
		return;
	} else {
		handle_interior();
		return;
	}
}


void handle_border()
{
	FragColor = vec4(colorBorderValue.x, colorBorderValue.y, 
						colorBorderValue.z, colorBorderValue.a * globalAlpha);
	return;
}


void handle_interior()
{
	if (use_texture_color != 0)
	{
		vec2 position = relative_position + vec2(width/2.0f - borderWidth, width/2.0f - borderWidth);
		vec2 uv = vec2(position.x / tex_width, position.y / tex_height);
		FragColor = texture2D(tex_color, uv);
		FragColor.a *= globalAlpha;

	} else {
		FragColor = vec4(colorValue.x, colorValue.y, colorValue.z, colorValue.a * globalAlpha);
	}
}
