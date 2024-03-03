
#include "Scene.h"

#include <cstdint>
#include <iostream>

#include "../../util/logging.h"
#include "../../window/window.h"
#include "../../window/FrameRateEnforcer.h"
#include "../../window/record.h"
#include "AnimationInstruction.h"

#include "glm/gtc/type_ptr.hpp"

#include "../../util/timing.h"

static bool enoughtime(uint64_t frame, float time){
    if (time < 0.0f){
        return false;
    }
    return frame >= seconds(time);
}

Scene::Scene(const Config& config, ColorObject bg) : 
    config(config),
    cam(config.width, config.height)
{
    bg_col[0] = bg.r; bg_col[1] = bg.g; bg_col[2] = bg.b;
    __set_fps(config.fps);
    wind = initialize(config.width, 
                        config.height, 
                        config.name.c_str(), 
                        config.fullscreen, 
                        config.fps, 
                        !config.show);
    shaders.initialize(config);

}

Scene::~Scene()
{
    timeline.deleteAll();
    terminate((GLFWwindow*)wind);
}

void Scene::generate_video(float maxtime)
{
    GLFWwindow* window = (GLFWwindow*)wind;
    logging::debug("Scene::generate_video", "Starting Initialization.");
    //if (window == NULL) return;// -1;

    FrameRateEnforcer frate_enf(config.fps);

    if(config.record)initialize(config);
    
    float t = glfwGetTime();
    uint64_t frame = 0;
    int nActiveElt;
    shapes.freeze();
    frate_enf.startloop();

    logging::debug("Scene::generate_video", "Initialization done.");
	while((!glfwWindowShouldClose(window) && !enoughtime(frame, maxtime))) //frame < seconds(5)) //
    {
       
        AnimationInstruction** current = timeline.getActiveEvents(frame, nActiveElt);
        for (int i = 0; i < nActiveElt; i++)
        {
            handle_event(current[i], frame);
        }
        delete[] current;

        for (TexturedColor* tex: effects)
        {
            tex->draw(frame);
        }
        
        backgroundColor(window, bg_col[0], bg_col[1], bg_col[2], 1.0f);

        while (!shapes.isempty())
        {
            shapes.pop()->draw(frame, get_camera().get_matrix());
        }

        if(config.record)save_frame(config);

        glfwSwapBuffers(window);
        shapes.restore();
        frate_enf.wait();
        if (checkExit(window)){
        	break;
        }
        
        glfwPollEvents();
        frame++;
    }
    float time = glfwGetTime() - t;
    std::cout << "Time taken: " << time  << " seconds."<< std::endl;
    std::cout << "FPS: " << frame / time << std::endl;

    if(config.record)finish();
}


void Scene::show()
{

}

void Scene::play(uint64_t frames)
{

}

void Scene::register_event(AnimationInstruction* a){
    timeline.addEvent(a->start, a->end, *a);
}


ShapeID Scene::register_shape(Shape& s)
{
    shapes.add(&s);
    return ++num_shapes;
}

void Scene::register_effect(TexturedColor &eff)
{
    effects.push_back(&eff);
}


void Scene::handle_event(AnimationInstruction* a, uint64_t frame)
{
    bool fresh = (a->animation_type & FRESHEVENT) != 0;
    a->animation_type = (~FRESHEVENT) & a->animation_type; // disable it being the first time handling
    
    float time;
    if (a->start == a->end) time = 1.0f;
    else time = float(frame - a->start)/(a->end - a->start);
    InterpolationFunction f = (InterpolationFunction)a->interpolFunc;

    switch(a->animation_type) 
    {
        case (MOVE):
        {
            // if this is the first time that we handle this particular event,
            // we need to actually input the starting locations for proper interpolation
            if (fresh)
            {
                int curr_x = a->shape->get_x();
                int curr_y = a->shape->get_y();

                a->data.move.xy[0] = curr_x;
                a->data.move.xy[1] = curr_y;
            }

            int start_x = a->data.move.xy[0];
            int start_y = a->data.move.xy[1];
            int target_x = a->data.move.xy[2];
            int target_y = a->data.move.xy[3];

            a->shape->set_position(f(start_x, target_x, time), 
                                    f(start_y, target_y, time));
            break;

        } 
        case (ROTATE):
        {
            if (fresh)
            {
                float curr_phi = a->shape->get_rot();
                a->data.rot.phi[0] = curr_phi;
            }

            float start_phi = a->data.rot.phi[0];
            float end_phi   = a->data.rot.phi[1];

            a->shape->set_rot(f(start_phi, end_phi, time));
            break;
        }
        case (SCALE):
        {
            if (fresh)
            {
                float curr_a = a->shape->get_scalef();
                a->data.scal.alpha[0] = curr_a;
            }

            float start_a = a->data.scal.alpha[0];
            float end_a   = a->data.scal.alpha[1];

            a->shape->set_scalef(f(start_a, end_a, time));
            break;
        }
        case(SETOPACITY):
        {
            if (fresh)
            {
                float curr_a = a->shape->get_opacity();
                a->data.opac.alpha[0] = curr_a;
            }

            float start_a = a->data.opac.alpha[0];
            float end_a   = a->data.opac.alpha[1];

            a->shape->set_opacity(f(start_a, end_a, time));
            break;
        }
        default:
            logging::error("Scene::handle_event", "Unknown event.");
    
    }
}