#version 330 core

/**
* Based on
* https://www.geeks3d.com/20101228/shader-library-frosted-glass-post-processing-shader-glsl/2
*/

in vec2 position;
out vec4 FragColor;

uniform sampler2D image;

uniform float wave_distortion = 0.008; // higher gives more distortion
uniform float wave_density = 2.1; // higher gives more waves
uniform vec4 ctrlpts = vec4(92.0f, 80.0f, 41.0f, 62.0f);

float smirk(vec2 pos)
{
    return fract(sin(dot(pos.xy , ctrlpts.rg)) + 
        cos(dot(pos.xy, ctrlpts.ba)) * wave_density);
}

/**
* The idea here with the next two functions is the fallowing:
*
* We use for the effect the texture coordinate (tc). This is to make sure
* that if the image moves, the effect does not change (it "moves" with the image).
* However, tc scale is relative to the size of the image. Hence, there would be a difference
* for small and large images. Therefore, we project the tc coordinates to uv coordinates
* of an imaginary 1920x1080 screen. I chose not to use the actual screen to 
* ensure that the variables maintain the same meaning and allow the user to
* render at low resolution for testing without suprises when switching resolution.
*
* These imaginary uv coordinates are as if the image was in the middle
* (the center of the image is in the middle) of this 1920x1080 screen.
*/

// convert from tex coord to imaginary screen
vec2 small_to_magnified_uv(vec2 tc)
{
    return vec2((tc.x * 2.0f - 1.0f) * textureSize(image, 0).x/1920.0f , 
        (tc.y * 2.0f - 1.0f) * textureSize(image, 0).y/1080.0f);
}

// imaginary uv to tc
vec2 magnified_to_small_tc(vec2 uv)
{
    return vec2((uv.x * 1920.0f/textureSize(image, 0).x) * 0.5f + 0.5f,
        (uv.y * 1080.0f/textureSize(image, 0).y) * 0.5f + 0.5f);
} 

void main()
{
    vec2 uv = small_to_magnified_uv(position);
    vec2 displacement = vec2(smirk(uv.xy), smirk(uv.yx));
    FragColor = texture(image, magnified_to_small_tc(uv + displacement * wave_distortion));
}

/*
older version
#version 330 core


in vec2 position;
out vec4 FragColor;

uniform sampler2D image;

uniform float wave_distortion = 0.005; // higher gives more distortion
uniform float wave_density = 5.1; // higher gives more waves
uniform vec2 v1 = vec2(92.,80.);
uniform vec2 v2 = vec2(41.,62.);

float smirk(vec2 pos)
{
    return fract(sin(dot(pos.xy ,v1)) + cos(dot(pos.xy,v2)) * wave_density);
}

vec2 small_to_magnified_uv(vec2 uv)
{
    return vec2(uv.x * textureSize(image, 0).x/1920.0f, 
        uv.y * textureSize(image, 0).y/1080.0f);
}

vec2 magnified_to_small_uv(vec2 uv)
{
    return vec2(uv.x * 1920.0f/textureSize(image, 0).x,
        uv.y * 1080.0f/textureSize(image, 0).y);
} 

void main()
{
    vec2 uv = small_to_magnified_uv(position);
    vec2 displacement = vec2(smirk(uv.xy), smirk(uv.yx));
    FragColor = texture(image, magnified_to_small_uv(uv + displacement * wave_distortion));
}*/