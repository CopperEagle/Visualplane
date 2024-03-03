
#ifndef window_h
#define window_h
/*!
 * @brief Window creation utility.
 **/

#include "../dependencies/glad.h"
#include <GLFW/glfw3.h>


GLFWwindow* initialize(int width, int height, const char* name, bool fullscreen = false, int fps = 60, bool offscreen = false);
void terminate(GLFWwindow* window);
void enableVSync(GLFWwindow* window);
void backgroundColor(GLFWwindow* window, float r, float g, float b, float alpha);
bool checkExit(GLFWwindow* window);

#endif