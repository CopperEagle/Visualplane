#version 330 core


in vec2 position;
out vec4 FragColor;

uniform int effects;
uniform int radius_boxblur;
uniform float flash_mul;
uniform sampler2D tex_color;

const int GREYSCALE = 1;
const int INVERT = 2;
const int BOXBLUR = 4;


void main()
{
	if ((effects & BOXBLUR) != 0)
    {
        FragColor = vec4(0.0f);
        vec2 dx = vec2(1.0f/textureSize(tex_color, 0).x, 0.0f);
        vec2 dy = vec2(0.0f, 1.0f/textureSize(tex_color, 0).y);
        float d = flash_mul/((radius_boxblur * 2.0f + 1.0f)*(radius_boxblur * 2.0f + 1.0f));
        for (int i = -radius_boxblur; i <= radius_boxblur; i++)
        {
            for (int j = -radius_boxblur; j <= radius_boxblur; j++)
            {
                FragColor += texture2D(tex_color, position + i*dx + j*dy)*d;
            }
        }
    } else {
        FragColor = texture2D(tex_color, position)*flash_mul;
    }

    if (FragColor.a < 0.00001) return;
    FragColor /= sqrt(FragColor.a);
    
    if ((effects & INVERT)  != 0)
    {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 0.0f) - FragColor;
        FragColor.a = -FragColor.a;
    }
    if ((effects & GREYSCALE) != 0)
    {
        //FragColor = vec4(vec3((FragColor.r + FragColor.g + FragColor.b)/3.0f),
        //           FragColor.a);
        // Formula: https://www.baeldung.com/cs/convert-rgb-to-grayscale
        float lum = 0.3f*FragColor.r + 0.59f*FragColor.g + 0.11*FragColor.b;
        FragColor = vec4(lum, lum, lum, FragColor.a);
    }
    
}
