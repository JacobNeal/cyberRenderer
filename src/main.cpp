#include "Window/GLFWWindow.hpp"
#include "Services/Renderer.hpp"

LOGGER_DECL_LIVE

int main()
{
    ce::GLFWWindow window("test", 800, 640);

    ce::IRenderer * renderer = new ce::Renderer;
    ce::RendererLocator::provide(renderer);

    GLuint shader = renderer->createShaderProgramFromFiles("../resources/shaders/shape/vertex.glsl", "../resources/shaders/shape/fragment.glsl");
    GLuint vao = renderer->createRect(glm::vec2(-0.5f, -0.5f), glm::vec2(0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f));

    while (!window.isDone())
    {
        window.begin();

        glUseProgram(shader);
        renderer->drawArrays(vao, 0, 6);

        GLenum err = glGetError();
        while((err = glGetError()) != GL_NO_ERROR) LOG("OpenGL Error: " + std::to_string(int(err)));

        window.end();
    }

    delete ce::RendererLocator::getRenderer();
    delete ce::EventLocator::getEventQueue();

    return 0;
}