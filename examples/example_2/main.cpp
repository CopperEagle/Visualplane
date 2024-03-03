
// Basic rendering
#include "visualplane.h" 
// Postprocessing effects
#include "visualplane-ppx.h" 
// No, there are no further headers from visualplane
// that you need to worry about.

// Explanation see example_1.
const Config config = Config {
    .name = "example_2",
    .width=720,
	.height=480,
	.fps=30,
	.fullscreen=false,

	.show=true,
	.record=true,
	.use_H265=false,
	.preset="ultrafast",
	.video_format="mp4",

	.fonts_dir="../../rsc/fonts/",
    .shaders_dir="../../rsc/"

};


int main(int argc, char const *argv[])
{
    
    Scene s(config, BLACK(1.0f));

    // Demo: More evolved example that involves 
    // playing with opacities, paths and text.
    Path p(s, 15, NO_SCREEN_RENDER);
    p.add_point(10, 10);
    p.add_point(280, 10);
    p.add_point(280, 280);
    p.add_point(10, 280);
    p.close();
    p.set_color(WHITE(0.1f));
   
    TextLine txt(0, 10, "Yes", s, "Comic_Sans_MS_Bold.ttf",92, NO_SCREEN_RENDER);
    txt.set_color(BLACK(1.0f));
    txt.set_outer_color(WHITE(1.0f));
    txt.enable_outline(4);
    txt.enable_italics();
    txt.set_opacity(0.1f);


    Square q(120, 120, 120, s, NO_SCREEN_RENDER);
    q.set_color(GREEN(0.6f));
    q.set_border(WHITE(0.8f), 1);
    q.rotate(3.14, seconds(2), seconds(4));

    Square q2(180, 180, 120, s, NO_SCREEN_RENDER);
    q2.set_color(MARGENTA(0.5f));
    q2.set_border(WHITE(0.8f), 1);

    Square q3(120, 120, 120, s, NO_SCREEN_RENDER);
    q3.set_color(PROCESSBLUE(0.0f));
    q3.set_border(WHITE(0.8f), 1);


    Square q4(180, 180, 120, s, NO_SCREEN_RENDER);
    q4.set_color(MARGENTA(0.0f));
    q4.set_border(WHITE(0.8f), 1);
    

    TexturedColor tcol(300, 300, s, true);
    tcol.start_color(3);
    tcol.add_color(0, 30, {0.0f, 1.0f, 1.0f, 1.0f});
    tcol.add_color(300, 300, {1.0f, 0.0f, 1.0f, 1.0f});
    tcol.add_color(30, 0, {0.0f, 1.0f, 1.0f, 1.0f});
    tcol.end_color();
    tcol.add_shape(txt);
    tcol.add_shape(p);
    tcol.add_shape(q);
    tcol.add_shape(q2);
    tcol.add_shape(q3);
    tcol.add_shape(q4);
    tcol.freeze();
    

    Square win(config.width/3, config.height/2, 300, s, 0);
    win.set_border(WHITE(1.0f), 5);
    win.set_color(tcol);

    win.set_opacity(0.0f);
    win.scale(0.0f, seconds(0), seconds(0));

    win.set_opacity(1.0f, seconds(0.1), seconds(1.5), ease);
    win.scale(1.0f, seconds(0.1), seconds(1.5), ease);


    q3.rotate(3.14, seconds(2), seconds(4), ease);
    win.move(config.width*2/3, config.height/2, seconds(1), seconds(3), ease);
    win.set_opacity(0.0f, seconds(4), seconds(5), ease);
    win.scale(1.2f, seconds(2), seconds(2.2), ease);
    win.scale(1.0f, seconds(2.1), seconds(2.5), linear);

    s.generate_video(5);
    return 0;
}
