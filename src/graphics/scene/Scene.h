#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <cstdint>


#include "../Camera.h"
#include "../2D/Shape.h"
#include "../../datastructures/IntervallTree.h"
#include "../../datastructures/MinHeap.h"
#include "../core/color/TexturedColor.h"
#include "../core/color/ColorObject.h"
#include "AnimationInstruction.h"
#include "ShaderKeeper.h"
#include "../../config.h"
/**
* What is the idea over here? Well...
* It should look like this:

int main()
{
    Scene s(...);
    Triangle t1(1, 2, BLUE, s);
    Triangle t2(2, 3, GREEN, s);

    move(t1, 2, 3, s, duration = 2); // t1.move(2, 3, s.now(), s.now() + seconds(1));

    s.play(10); // the triangles stand there for 10 seconds
                // s.now() == 10s

    Triangle t3(1, 2, BLACK, s);
    ColorGradient col(GREEN, BLUE, Vector(1, 2));
    t3.set_color(col, s.now(), s.now() + second(5));

    Triangle t4(1, 2, YELLOW, s);
    fade_in(t4, s, duration = 1);

    s.play(10);

    s.generate("output.mp4");

}*/

/*!
* Effects (TexturedColors): The order in ewhich they are drawn is the order in which they are submitted.*/

class Scene
{
public:
    Scene(const Config& config, ColorObject background_color);
    ~Scene();
    uint64_t now(){return 0;};

    ShapeID register_shape(Shape& s);
    void register_effect(TexturedColor& eff);
    // negative: exit only upon pressing q
    void generate_video(float maxtime = -1.0f);
    void show();
    void play(uint64_t frames);

    void register_event(AnimationInstruction* a); // 	timeline.addEvent(start, end, *a);

    const Camera& get_camera(){return cam;};

    ShaderKeeper& get_shaders(){return shaders;};

    const Config& get_config(){return config;}


private:
    void handle_event(AnimationInstruction* a, uint64_t frame);

    ShaderKeeper shaders;
    Camera cam;
    IntervallTree<AnimationInstruction> timeline;
    MinHeap shapes;
    std::vector<TexturedColor*> effects;
    const Config& config;
    void* wind;
    uint64_t num_shapes = 0;
    float bg_col[3];
};


#endif // SCENE_H