#ifndef CE_OPENGL_HPP
#define CE_OPENGL_HPP

//#define USE_SFML_WINDOW
#define USE_GLFW_WINDOW

#ifdef USE_SFML_WINDOW
//#include <SFML/OpenGL.hpp>
#include <OpenGL/gl3.h>
#endif

#ifdef USE_GLFW_WINDOW
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#endif

#endif