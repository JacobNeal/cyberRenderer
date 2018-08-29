////////////////////////////////////////////////////////////
//
// cyberEngine
// The MIT License (MIT)
// Copyright (c) 2018 Jacob Neal
//
// Permission is hereby granted, free of charge, to any person 
// obtaining a copy of this software and associated documentation 
// files (the "Software"), to deal in the Software without restriction, 
// including without limitation the rights to use, copy, modify, merge, 
// publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be 
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////
#include "Window/GLFWWindow.hpp"

namespace ce
{

//////////////////////////////////////////////////////////////
GLFWWindow::GLFWWindow(const char * title, const GLuint & width, const GLuint & height)
    : Window(width, height)
{
    //////////////////////////////////////////////
    // Set up GLFW
    //////////////////////////////////////////////
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    //////////////////////////////////////////////
    // Create the GLFW window for rendering.
    //////////////////////////////////////////////
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (m_window == nullptr)
    {
        LOG("Failed to create the GLFW framework window.");
        glfwTerminate();
    }

    //////////////////////////////////////////////
    // Tell GLFW to make the context of our window 
    // the main context on the current thread.
    //////////////////////////////////////////////
    glfwMakeContextCurrent(m_window);

    //////////////////////////////////////////////
    // Set up GL3W to initialize OpenGL
    //////////////////////////////////////////////
    if (gl3wInit())
        LOG("Failed to initialize GL3W.");
    
    if (!gl3wIsSupported(3, 3))
        LOG("OpenGL 3.3 is not supported.");
    
    LOG("OpenGL " + std::string((char *)glGetString(GL_VERSION)) + ", " +
        "GLSL " + std::string((char *)glGetString(GL_SHADING_LANGUAGE_VERSION)));

    //////////////////////////////////////////////
    // Tell OpenGL the size of the rendering window.
    //////////////////////////////////////////////
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) LOG("OpenGL Error: " + std::to_string(int(err)));

    int windowXPos, windowYPos;
    glfwGetWindowPos(m_window, &windowXPos, &windowYPos);
    glfwSetWindowPos(m_window, windowXPos + 1, windowYPos);

    //////////////////////////////////////////////
    // Set the GLFW key callback for the window.
    //////////////////////////////////////////////
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, windowKeyCallback);

    //////////////////////////////////////////////
    // Set the GLFW mouse move callback for the window.
    //////////////////////////////////////////////
    glfwSetCursorPosCallback(m_window, windowMouseMoveCallback);

    // Set all controls to a default state of released.
    for (bool & control : m_controls) control = false;

    while((err = glGetError()) != GL_NO_ERROR) LOG("OpenGL Error: " + std::to_string(int(err)));

    ce::EventLocator::initialize();
}

//////////////////////////////////////////////////////////////
GLFWWindow::~GLFWWindow()
{
    glfwTerminate();
}

//////////////////////////////////////////////////////////////
bool GLFWWindow::isDone()
{
    //assert(glGetError() == GL_NO_ERROR);
    return glfwWindowShouldClose(m_window) > 0;
}

//////////////////////////////////////////////////////////////
GLfloat GLFWWindow::getElapsedTime()
{
    /////////////////////////////////////////////////
    // Update the elapsed time since the last frame
    /////////////////////////////////////////////////
    GLfloat currentFrameTime = glfwGetTime();
    m_elapsedTime = currentFrameTime - m_lastFrameTime;
    m_lastFrameTime = currentFrameTime;
    
    return m_elapsedTime;
}

//////////////////////////////////////////////////////////////
void GLFWWindow::begin()
{
    glfwPollEvents();
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//////////////////////////////////////////////////////////////
void GLFWWindow::end()
{
    glfwSwapBuffers(m_window);
}

//////////////////////////////////////////////////////////////
void GLFWWindow::windowKeyCallback(GLFWwindow * window, int key, int scanCode, int action, int mode)
{
    GLFWWindow * ceWindow = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
    ceWindow->keyCallback(action, key);
}

//////////////////////////////////////////////////////////////
void GLFWWindow::keyCallback(int action, int key)
{
    if (action == GLFW_PRESS || action == GLFW_RELEASE)
    {
        bool pressed = action == GLFW_PRESS;

        if (m_controls[key] != pressed)
        {
            std::string eventType;

            switch (key)
            {
                case GLFW_KEY_UP:
                    eventType = "Up";
                    break;
                case GLFW_KEY_DOWN:
                    eventType = "Down";
                    break;
                case GLFW_KEY_LEFT:
                    eventType = "Left";
                    break;
                case GLFW_KEY_RIGHT:
                    eventType = "Right";
                    break;

                default:
                    eventType = std::string(1, char(key));
            }

            Event event(eventType);
            event.eventBool = pressed;

            EventLocator::getEventQueue()->addEvent(event);
            m_controls[key] = pressed;
        }
    }
}

//////////////////////////////////////////////////////////////
void GLFWWindow::windowMouseMoveCallback(GLFWwindow * window, double xPos, double yPos)
{
    GLFWWindow * ceWindow = static_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
    ceWindow->mouseMoveCallback(xPos, yPos);
}

//////////////////////////////////////////////////////////////
void GLFWWindow::mouseMoveCallback(double xPos, double yPos)
{
    m_mousePosition.x = xPos;
    m_mousePosition.y = m_height - yPos;
}

//////////////////////////////////////////////
void GLFWWindow::windowMouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{

}

//////////////////////////////////////////////
void GLFWWindow::mouseButtonCallback(int button, int action)
{

}

} // namespace ce