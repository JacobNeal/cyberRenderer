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

#ifndef CE_GLFW_WINDOW_HPP
#define CE_GLFW_WINDOW_HPP

//////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////
#include "Window.hpp"
#include "Logger.hpp"
#include "EventQueue.hpp"

namespace ce
{

class GLFWWindow : public Window
{
    public:
        GLFWWindow(const char * title, const GLuint & width, const GLuint & height);
        virtual ~GLFWWindow();

        bool isDone();
        GLfloat getElapsedTime();
        void begin();
        void end();

    private:
        static void windowKeyCallback(GLFWwindow * window, int key, int scanCode, int action, int mode);
        void keyCallback(int action, int key);

        static void windowMouseMoveCallback(GLFWwindow * window, double xPos, double yPos);
        void mouseMoveCallback(double xPos, double yPos);

        static void windowMouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
        void mouseButtonCallback(int button, int action);

        //////////////////////////////////////////////////////////////
        // Data members
        //////////////////////////////////////////////////////////////
        GLFWwindow * m_window;
        GLfloat      m_elapsedTime;
        GLfloat      m_lastFrameTime;

        bool         m_controls[256];
};

} // namespace ce

#endif