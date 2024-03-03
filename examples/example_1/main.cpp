
// Basic rendering
#include "visualplane.h" 
// Postprocessing effects
#include "visualplane-ppx.h" 
// No, there are no further headers from visualplane
// that you need to worry about.


const Config config = Config {
    .name = "example_1",    // name of project
    .width=1280,            // width of window, video
	.height=720,            // height of window, video
	.fps=30,                // displayed FPS. Video is guaranteed to have this FPS, even if it lags while generating.
	.fullscreen=false,      // for large renderings, ignore if show=false

	.show=true,             // Show scene in window.
	.record=true,           // Record to video.
	.use_H265=false,        // Encoding. True for smaller video file size.      
	.preset="ultrafast",    // FFmpeg setting. You can ignore it.
	.video_format="mp4",    // mov or mp4. (Webm is buggy.)

	.fonts_dir="../../rsc/fonts/",  // directory of your fonts
    .shaders_dir="../../rsc/"       // directory of shaders that are in this project

};


int main(int argc, char const *argv[])
{
    
    Scene s(config, BLACK(1.0f));

    // Demoing the use of TexturedColors
    /**
    * In addition to usual colors, there exist TexturedColors. These are little render spaces
    * on which you can draw any shape and then add post processing effects.
    * The render of that space can then be used as a color for other shapes.
    * Note that not every shape supports this.*/

    // Demo: Adding post processing effects 
    // This demo draws an image onto a TexturedColor and then adds a postprocessing effect.
    
    // First create the "color"
    // width, height, the scene and live=true.
    // live means that if any object on the TexturedColor scene
    // changes (animation), this will be reflected.
    TexturedColor img_surface(600, 600, s, true);
    

    // The image. Filepath, x, y of image middle,
    // width, height, the scene and the z_layer.
    // Important: We don't want it to be visible on the scene itself
    // (Should only be on the TexturedColor above). 
    // Thus we need to set z_layer to NO_SCREEN_RENDER. 
    // The order in which objects in the
    // TexturedColor are rendered is the order of insertion.
    Image img("winter.png", 300, 300, 
        512, 512, s, NO_SCREEN_RENDER);

    // Animations for the image itself.
    img.scale(0.6f, seconds(2), seconds(3.5), ease);
    img.move(200, 200, seconds(2), seconds(3.5), ease);
    img.move(200, 400, seconds(4), seconds(5.5), ease);
    img.move(400, 300, seconds(6), seconds(7.5), ease);

    img.move(300, 300, seconds(8), seconds(9), ease);
    img.scale(1.0f, seconds(8), seconds(9), ease);
    img.set_opacity(0.0f, seconds(11), seconds(12), ease);

    // Render the image on the TexturedColor.
    // Since img_surface is live, the animations above will be reflected.
    img_surface.add_shape(img);

    // Adding post processing effects to the TexturedColor
    FrozenCurveEffect feffect(config);
    img_surface.append_postproc_effect(feffect);
    // There is also blur, greyscale, invert colors
    img_surface.add_effect_flash(2);
    
    // After freezing the TexturedColor, no more objects should be added.
    img_surface.freeze(); 

    // Next the window that will use this TexturedColor.
    // Remember TexturedColor is basically a "color" for another shape.
    Square window(config.width/2, config.height/2, 600, s);
    window.set_border(WHITE(0.0f));
    window.set_color(img_surface);

    // The time is in seconds. Negative means arbitrary length.
    // Press q to stop the scene.
    s.generate_video(12);
    return 0;
}
