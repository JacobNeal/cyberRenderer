#include "Window/GLFWWindow.hpp"
#include "Services/Renderer.hpp"

LOGGER_DECL_LIVE

int main()
{
    ce::GLFWWindow window("test", 800, 640);

    ce::IRenderer * renderer = new ce::Renderer;
    ce::RendererLocator::provide(renderer);

    GLuint voxelShader = renderer->createShaderProgramFromFiles("../resources/shaders/cube/vertex.glsl", "../resources/shaders/cube/fragment.glsl");
    GLuint voxel       = renderer->createVoxel(glm::vec3(0.0f, 0.0f, 0.0f),     // Top - Left - Front position
                                               glm::vec3(10.0f, 10.0f, 10.0f),  // Size
                                               glm::vec3(1.0f, 0.0f, 0.0f));    // Color

    glm::vec3 cameraPosition(0.0f, 0.0f, 0.0f);

    while (!window.isDone())
    {
        window.begin();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(200, 160, 0));
        model = glm::rotate(model, (float)glm::radians(glfwGetTime() * 60), glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 view       = glm::lookAt(cameraPosition, cameraPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::ortho(0.0f, 400.0f, 0.0f, 320.0f, -100.0f, 100.0f);
        glm::mat4 mvp = projection * view * model;

        glUseProgram(voxelShader);
        renderer->passUniformMatrix(voxelShader, "mvp", mvp);
        renderer->drawArrays(voxel, 0, 36);

        GLenum err = glGetError();
        while((err = glGetError()) != GL_NO_ERROR) LOG("OpenGL Error: " + std::to_string(int(err)));

        window.end();
    }

    delete ce::RendererLocator::getRenderer();
    delete ce::EventLocator::getEventQueue();

    return 0;
}