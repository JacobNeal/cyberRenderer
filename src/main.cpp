#include "Window/GLFWWindow.hpp"
#include "Services/Renderer.hpp"

LOGGER_DECL_LIVE

int main()
{
    ce::GLFWWindow window("test", 800, 640);

    ce::IRenderer * renderer = new ce::Renderer;
    ce::RendererLocator::provide(renderer);

    const char * vertexShader = 
        "#version 330 core\n"
        "layout (location=0) in vec3 inPos;\n"
        "layout (location=1) in vec3 inColor;\n"
        "out vec3 fragColor;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(inPos, 1.0);\n"
        "    fragColor = inColor;\n"
        "}\0";

    const char * fragmentShader = 
        "#version 330 core\n"
        "in vec3 fragColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(fragColor, 1.0);\n"
        "}\0";

    GLuint shader = renderer->createShaderProgram(vertexShader, fragmentShader);
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