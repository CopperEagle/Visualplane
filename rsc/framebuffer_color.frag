#version 330 core


in vec2 position;
out vec4 FragColor;


uniform int width;
uniform int height;
uniform int numColors;
uniform sampler1D color_points; // first all points, then all colors

// uniform int useBaseColor;
// uniform vec4 baseColor;

float len(vec2 x)
{
    return x.x * x.x + x.y * x.y;
}

vec4 getColor(int);
vec2 getPosition(int);

void main()
{
    vec4 col = vec4(0.0f);
    float dist = 0.0f;

    for (int i = 0; i < numColors; i++)
    {
        // float offset = 1;
        // if (i == 2)
        // {
        //     offset = 10;
        // }
        float d = len(getPosition(i) - position);//*offset;
        if (d < 0.0001){
            FragColor = getColor(i);
            return;
        }
        dist += 1.0f/d;
    }


    for (int i = 0; i < numColors; i++)
    {
        float d = 1.0f/len(getPosition(i) - position);
        // if (i == 2)
        // {
        //     d /= 10;
        // }
        float alpha = d/dist;
        
        col += getColor(i)*alpha;

    }

    FragColor = col;
	
}

vec4 getColor(int i) 
{
    int j = numColors*2 + 4*i;
    return vec4(texelFetch(color_points, j    , 0).r, 
                texelFetch(color_points, j + 1, 0).r,
                texelFetch(color_points, j + 2, 0).r,
                texelFetch(color_points, j + 3, 0).r);
}

vec2 getPosition(int i)
{
    return vec2(texelFetch(color_points, 2*i     , 0).r * width,
                texelFetch(color_points, 2*i + 1 , 0).r * height);
}