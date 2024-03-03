#version 330 core

// in vec2 position;
in vec2 position;
out vec4 fragColor;

uniform float time;
uniform sampler2D image;

uniform float wave_length = 2.0f;
uniform float amplitude = 0.05f;

void main()
{
    vec2 newpos = position;

    float len = length(newpos - vec2(.5f)) * 50;
  
    vec2 s = normalize(newpos) * ( sin((len - time * 10.0f))) * amplitude;      
    fragColor = texture(image, newpos + s);
    
}






// uniform vec2 iMouse = vec2(700.0f, 700.0f);
// uniform vec2 iResolution = vec2(800, 800);

// float AMT_FROST = 0.7f;

// float random (vec2 st) {
//     return fract(sin(dot(st.xy, vec2(23.534, 89.234)))* 52912.456);
// }

// vec2 myPattern(vec2 uv){
// 	vec2 uv2 = uv;
//     uv2.y = uv2.y + AMT_FROST * ((random(uv)) - 0.5);
//     uv2.x = uv2.x + AMT_FROST * ((random(uv)) - 0.5);
//     return uv2 - uv;
// }

// void main ()
// {
//   //vec2 fragCoord = position;
//   vec2 uv = position; //gl_FragCoord.xy / iResolution.xy;
//   vec2 p = uv;
//   for (int i = 0; i < 10; i++) p -= myPattern(p) * 0.03;
//   //vec3 col = texture(image, p).rgb;
//   //fragColor = vec4 (col, 1.);
//   fragColor = texture(image, p);
// }











// #define PI 3.1415936535

// float some(float x)
// {
//     x = x ;
//     return exp(- (x * x));
// }



// void main() {
//     vec2 fragCoord = position;
// 	vec2 screen_window = fragCoord / iResolution.x * 2.0 - 1.0; // x goes from [-1 to 1].
//     // This is used to accumulate over all ripples at the fragment location.
//     vec2 displacement = vec2(0.0);
//     // Same ripples over and over again, they repeat with this frequency.
//     float loop_time = 3.0;
//     // Lower numbers make wider ripples.
//     float ripple_width_inverse = 20.0;
//     float limits = PI / ripple_width_inverse;
//     // Increase decay to quicken fading of ripples with time.
//     float decay = 2.0;
//     // Controls the concentration of ripples (how far they spawn from the origin).
//     float spread = 4.0;
//     // Increase number of ripples. It can be cheaper to decrease the loop time.
//     int num_ripples = 68;
    
//     // Change the perspective from top down (straight) to an angular view.
//     //screen_window /= (screen_window.y += 1.6) - 2.00;
    
//     for (float i = 0.0; i < float(num_ripples); ++i) {
//         // Distance from the origin of a ripple.
//         vec2 origin = vec2(919.,154.) * i;
//         // The cos wraps the ripple spawn points to within the local area.
//         vec2 dist = screen_window - spread * (cos(origin) - 0.5);
//         // Increasing this value staggers the creation time of ripples. For zero, they all create at the
//         // same time.
//         float time_offset = i * 0.12;
//         float t = length(dist) - mod(time_offset + time, loop_time);
//         // Only show one period of each sine wave.
//         float t_one_period = clamp(t, -limits, limits);
//         // Waves decay with time. I've added a decay parameter to control this more as well as
//         // a small offset to prevent a divide by zero artifact.
//         vec2 amplitude = dist / (pow(dot(dist, dist), decay) + 0.1);
//         displacement += amplitude * some(ripple_width_inverse * t_one_period);        
//     }
//     // Original: use the displacement to offset the read from the texture.
//     fragColor =  texture(image, vec2( (1 + screen_window)/2 - displacement/6));
//             // Bottom of the pond. No magic here.
//             //0.4 * texture(iChannel1, screen_window);
//     // View only the displacement.
//     //fragColor = vec4(vec3(displacement + 0.5,0.5), 1);
// }




// void main()
// {
//     vec2 fragCoord = position;
// 	vec2 uv = fragCoord.xy / iResolution.xy;
    
//     vec3 shockParam = vec3(100.0, 1.8, 40.0); // Amplitude Effect, Refraction, width
    
//     float dis = distance(iMouse.xy, fragCoord.xy);
    
//     float speed = 280.0 + 100;
    
//     float timeShock = 4.0;
    
//     //current Radius of Shockwave
//     float currentRadius = mod(time, timeShock) * speed + mod(time, timeShock)*mod(time, timeShock)*(-50);
    
//     if(dis <= currentRadius + shockParam.z && dis >= currentRadius - shockParam.z)
//     {
//         float diff = (dis - currentRadius) / shockParam.z; //from -1.0 to 1.0
        
//         float diffPow = 1.0 - pow(abs(diff), shockParam.y);
        
//         //caculate current direction of pixel
//         vec2 dir = (fragCoord.xy - iMouse.xy);
//         dir = normalize(dir);
        
        
//         float diffFinal = diff * diffPow;
        
//         uv = (fragCoord.xy + dir * diffFinal * shockParam.x).xy / iResolution.xy;
//     }
    
// 	fragColor = texture(image, uv);
// }



// float strength(float maximum)
// {
//     if (time < 3){
//         return (0.33 * time)*maximum;
//     } else if ( time > 6){
//         return max(0.0f, -0.33*(time - 9))*maximum;
//     }
//     return maximum;
// }

// void main()
// {

//     float len = length(position - vec2(.5f)) * 50.0f*2;    
//     vec2 s = normalize(position) * ( sin((len - time * 10.0f))) * strength(.1f);      
//     vec4 tex = texture(image, position + s);       
//     fragColor = tex;
    
// }


// const float INNER_RADIUS_START = 0.75;
// const float OUTER_RADIUS_START = 0.9;
// const float PULSE_SIZE = 0.1;

// const float SHADOW_SIZE = 0.1;

// float INNER_RADIUS = 0.75;
// float OUTER_RADIUS = 0.9;
// const float SHEET_THICKNESS = 0.012;
// const float NOISINESS = 10.0;


// const int NUM_STEPS = 20;
// const float TIME_SCALE = 5.0;



// float trapezium(float x)
// {
//     //            __________
//     // 1.0 -     /          \
//     //          /            \                .
//     // 0.5 -   /              \              .  --> Repeating
//     //        /                \            . 
//     // 0.0 - /                  \__________/
//     //
//     //       |    |    |    |    |    |    |
//     //      0.0  1/6  2/6  3/6  4/6  5/6  6/6
//     //
// 	return min(1.0, max(0.0, 1.0 - abs(-mod(x, 1.0) * 3.0 + 1.0)) * 2.0);
// }

// vec3 colFromHue(float hue)
// {
//     // https://en.wikipedia.org/wiki/Hue#/media/File:HSV-RGB-comparison.svg
// 	return vec3(trapezium(hue - 1.0/3.0), trapezium(hue), trapezium(hue + 1.0/3.0));
// }

// // Cheap noise functions. I just messed around with sin functions until
// // I got something I liked. The important thing was to make sure the periods
// // of the sin functions weren't constant and varied over space.
// float cnoise3(float pos)
// {
// 	return (cos(pos / 2.0) * 0.2 + 1.0);
// }

// float cnoise2(float pos)
// {
// 	return (sin(pos * cnoise3(pos) / 2.0) * 0.2 + 1.0);
// }

// float cnoise(vec4 pos)
// {
//     // These values are all very carefully chosen using 
//     // lots of very complex mathematics. In other news, 
//     // bashing my head on my keyboard is now complex 
//     // mathematics
//     float x = pos.x * cnoise3(pos.y) + pos.w * 0.87123 + 82.52;
//     float y = pos.y * cnoise3(pos.z) + pos.w * 0.78725 + 12.76;
//     float z = pos.z * cnoise3(pos.x) + pos.w * 0.68201 + 42.03;
//     return (sin(x) + sin(y) + sin(z)) / 3.0;
// }

// vec4 merge_colours(vec4 apply_this, vec4 on_top_of_this)
// {
//     // Very basic colour merging
//     return on_top_of_this * (1.0 - apply_this.a) + apply_this * apply_this.a;
// }


// float get_radius(vec3 pos, float inner_radius, float outer_radius)
// {
//     float scaled_time = time * TIME_SCALE;
//     vec3 samplePos = normalize(pos);
//     // Now we're going to sample the noise function to get the radius of the bubble along this ray
//     float rel_rad = (cnoise(vec4(samplePos * NOISINESS, scaled_time)) + 1.0) / 2.0;
//     // Clamp the noise in case using a different noise function (perlin for example)
//     rel_rad = clamp(rel_rad, 0.0, 1.0);
//     // Calculate the inner and outer most radius boundaries
//     float innerIncBorder = inner_radius + SHEET_THICKNESS;
//     float outerIncBorder = outer_radius - SHEET_THICKNESS;
//     // Calculate the radius of the bubble by linearly interpolating 
//     // the noise sample between inner and outer boundaries.
//     float radius = innerIncBorder + (outerIncBorder - innerIncBorder) * rel_rad;
//     return radius;
// }


// vec4 getdensity(vec3 pos, float inner_radius, float outer_radius)
// {
//     // This function get's the "density" of fog at a position in space (pos)
    
//     // First, let's make a variable we can reuse for scaled time.
//     float scaled_time = time * TIME_SCALE;
    
//     // The next thing to do is decide where to sample the noise functions.
//     // We want the radius of the bubble to be constant along any ray from 
//     // the center of the bubble. So, to ensure that we always sample the same
//     // position in the noise function for any ray, we normalize the position
//     // vector (since the origin of the bubble is at 0)
//     vec3 samplePos = normalize(pos);
    
//     // The inner colour of the buble is just a random colour sampled from the cheap noise function.
//     vec4 inner_color = vec4(colFromHue(cnoise(vec4(samplePos / 5.0, scaled_time / 15.0))) * 25.0, 1.0);
//     // The outer colour of the buble is a big whiter than the inside. This helps make the bubble
//     // look more natural.
//     vec4 outer_color = merge_colours(vec4(25.0,25.0,25.0,0.5), inner_color);

    
//     // Now we're going to sample the noise function to get the radius of the bubble along this ray
//     float sample_ = (cnoise(vec4(samplePos * NOISINESS, scaled_time)) + 1.0) / 2.0;
//     // Clamp the noise in case using a different noise function (perlin for example)
//     sample_ = clamp(sample_, 0.0, 1.0);
//     // Calculate the inner and outer most radius boundaries
//     float innerIncBorder = inner_radius + SHEET_THICKNESS;
//     float outerIncBorder = outer_radius - SHEET_THICKNESS;
//     // Calculate the radius of the bubble by linearly interpolating 
//     // the noise sample between inner and outer boundaries.
//     float radius = innerIncBorder + (outerIncBorder - innerIncBorder) * sample_;
    
//     // Calculate the distance between the volume sample position and the center of the bubble
//     float dist = distance(pos, vec3(0.0, 0.0, 0.0));
//     // Calculate the density of the fog. We use a very "strongly peaking" function here. 
//     // It's almost 0 everywhere except at radius, where it peaks to 1 and then falls to 0 very quickly.
//     // Take a look at it in wolframalpha. 
//     float density = exp(-pow(dist - radius, 2.0) * 5000.0);
    
//     // Calculate final color here. Lerp the inner and outer colours depending on the radius and scale by density
//     return (inner_color + (outer_color - inner_color) * (radius - innerIncBorder) / (outerIncBorder - innerIncBorder)) * density;
// }

// vec4 raymarch(vec3 start, vec3 end, float inner_radius, float outer_radius)
// {
//     // This is the ray marching function. Here, we sample NUM_STEPS points along the vector
//     // between start and end. Then, we integrate the resultant densities linearly.
//     vec4 retn = vec4(0.0, 0.0, 0.0, 0.0);
// 	vec3 delta = end - start;
//     float stepDistance = length(delta) / float(NUM_STEPS);
    
//     vec4 densityPrevious = getdensity(start, inner_radius, outer_radius);
//     for (int i = 1; i < NUM_STEPS; i++) 
//     {
//         vec3 samplePos = start + delta * float(i) / float(NUM_STEPS);
//         vec4 density = getdensity(samplePos, inner_radius, outer_radius);
//         // Integrate the density using linear interpolation
//         // The colours will be the average of the two weighted by their alpha
//         vec4 densityIntegrated = (density + densityPrevious) / 2.0;
//         // Optimised out to return. densityIntegrated *= stepDistance
//         retn += densityIntegrated;
        
//         densityPrevious = density;
//     }
    
//     return retn * stepDistance;
// }



// vec4 raymarch_ball(vec2 coord, float inner_radius, float outer_radius)
// {

//     // Now we're going to intersect a ray from the 
//     // coord along the Z axis onto two spheres, one 
//     // inside the other (same origin). getdensity 
//     // is only > 0 between these volumes.
//     float d = distance(coord, vec2(0.0, 0.0));
//     if (d > outer_radius) {
//         // No intersection on the spheres.

//         return vec4(0.0f);
        
//     } 
//     float dOuterNormalized = d / outer_radius;
//     float outerStartZ = -sqrt(1.0 - dOuterNormalized*dOuterNormalized) * outer_radius; // sqrt(1-x*x) = function of a circle :)
//     float outerEndZ = -outerStartZ;
//     if (d > inner_radius) {
//         // The ray only intersects the larger sphere, 
//         // so we need to cast from the front to the back
        
//         // We do it twice so that the number of samples in this branch
//         // is identical to the number of samples 
//         // inside the blob. Otherwise we see artifacts with 
//         // a lower number of samples.
//         vec4 frontPart = raymarch(vec3(coord, outerStartZ), vec3(coord, 0), inner_radius, outer_radius);
//         vec4 backPart = raymarch(vec3(coord, 0), vec3(coord, outerEndZ), inner_radius, outer_radius);
//         return frontPart + backPart;
//     }
    
//     float dInnerNormalized = d / inner_radius;
//     float innerStartZ = -sqrt(1.0 - dInnerNormalized*dInnerNormalized) * inner_radius; // sqrt(1-x*x) = function of a circle :)
//     float innerEndZ = -innerStartZ;
//     // The ray intersects both spheres.
//     vec4 frontPart = raymarch(vec3(coord, outerStartZ), vec3(coord, innerStartZ), inner_radius, outer_radius);
//     vec4 backPart = raymarch(vec3(coord, innerEndZ), vec3(coord, outerEndZ), inner_radius, outer_radius);
//     vec4 final = frontPart + backPart;

//     return final;
// }



// float proximity_function(float a, float b, float x, float rad)
// {
//     if (a <= x && x <= b) return 1.0f;
//     if (a > x)
//     {
//         return max(0.0f, (rad - (a - x))/rad);
//     }
//     return max(0.0f, (rad - (x - b))/rad);
// }



// vec4 darken(vec4 col, vec2 pos)
// {
//     vec4 add_col = vec4(0.0f, 0.0f, 0.0f, 1.0f);
//     float rad = get_radius(vec3(pos, 0), INNER_RADIUS + 0.03, OUTER_RADIUS + 0.03);
//     float t = proximity_function(rad - 0.1, rad, distance(pos, vec2(0.0, 0.0)), SHADOW_SIZE);

//     return t*add_col + (1-t)*col;
// }




// void main()
// {
//     INNER_RADIUS = INNER_RADIUS_START + sin(time) * PULSE_SIZE;
//     OUTER_RADIUS = OUTER_RADIUS_START + sin(time) * PULSE_SIZE;
//     vec2 res = textureSize(image, 0);
// 	//vec2 uv = (fragCoord.xy / min(res.x, res.y)) * 2.0 - vec2(res.x / res.y, 1.0);
//     vec2 uv = (position * 2.0f - vec2(1.0f));
//     fragColor = merge_colours(raymarch_ball(uv, INNER_RADIUS, OUTER_RADIUS), darken(texture(image, position), uv));//vec4(0.0, 0.0, 0.0, 1.0));

// }


