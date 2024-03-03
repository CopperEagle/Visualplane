#include "window.h"
#include <GLFW/glfw3.h>
#include "../util/logging.h"

/*!
 * @brief Checks whether the dimensions of the window are as supposed.
 * There are some conditions when the actual dimensions actually differ.
 **/
using logging::info;

static void check(int width, int height) {
  GLFWmonitor *primary = glfwGetPrimaryMonitor();
  const GLFWvidmode *vid = glfwGetVideoMode(primary);
  if (vid->height != height) {
    info("window.cpp::check", "Could not create window of height " +
            std::to_string(height) + " got " +
            std::to_string(vid->height) + " instead.");
  }
  if (vid->width != width) {
    info("window.cpp::check", "Could not create window of width " +
            std::to_string(width) + " got " +
            std::to_string(vid->width) + " instead.");
  }
}

/*!
* @brief Creates a window using glfw.
* 
* @param[in] width of the window
* @param[in] height of the window
* @param[in] name the title of the window 
* @param[in] borderless Whether the window should have a top border (use for fullscreen)
* @param[in] fps a hint for when borderless is true (currently). PROBABLY IGNORED BY GLFW/OS,
* 
* @return Pointer to the window struct if successful, else NULL.
*
* @note This currently still enforces the standard blending alpha, 1-alpha, rather than additive
* blending (which is what you want in some cases.)
* 
* \todo Create a global setting that sets whether one uses additive or std blending and describe the differences.
**/
GLFWwindow* initialize(int width, int height, const char* name, bool fullscreen, int fps, bool offscreen)
{
    if (offscreen) fullscreen = false;
    
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    if (offscreen)
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    if (fullscreen)
    {
        glfwInitHint(GLFW_REFRESH_RATE, fps);
    }

    GLFWwindow* window =  glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == NULL)
    {
        info("window.cpp::initialize", "Window cannot be created."); // I think this should not be here.
        glfwTerminate();
        return NULL;
    }

    if (fullscreen)
    {
        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        glfwSetWindowMonitor(window, primary, 0, 0, width, height, fps);
    }

    // tell glfw that we use this window to draw on
    glfwMakeContextCurrent(window);

    // initialize OpenGL
    gladLoadGL();

    // specify the region to draw on
    glViewport(0, 0, width, height);
    
    // disable VSync by default
    glfwSwapInterval(0);

    // enable alpha channel
    glEnable(GL_BLEND);     
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if (fullscreen)
        check(width, height);

    return window;
}

/*!
* @brief Sets the background color of the window. r, g, b, a must all be floats between
* zero and one.
**/
void backgroundColor(GLFWwindow* window, float r, float g, float b, float alpha)
{
    glClearColor(r, g, b, alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}

/*!
* @brief Restricts the number of updates per second to half the fps of the monitor.
**/
void enableVSync(GLFWwindow* window)
{
    glfwSwapInterval(2); // one update every two frames. results in 30 fps here
}

/*!
* @brief Closes the window and shuts down the glfw engine.
**/
void terminate(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

/*!
* @brief Returns true if the user presses q
**/
bool checkExit(GLFWwindow* window)
{
    return (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS);
}