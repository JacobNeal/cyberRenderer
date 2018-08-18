#include "Window/GLFWWindow.hpp"
#include "Services/Renderer.hpp"

LOGGER_DECL_LIVE

int main()
{
    ce::GLFWWindow window("test", 800, 640);

    ce::IRenderer * renderer = new ce::Renderer;
    ce::RendererLocator::provide(renderer);

    GLuint voxelShader = renderer->createShaderProgramFromFiles("../resources/shaders/cube/vertex.glsl", "../resources/shaders/cube/fragment.glsl");

    unsigned int numVoxels = 10;
    unsigned int voxelSize = 50;
    GLuint voxels[numVoxels];

    for (unsigned int count = 0; count < numVoxels; ++count)
    {
        GLfloat red   = count % 2 == 0 ? 1.0f : 0.0f;
        GLfloat blue  = count % 3 == 0 ? 1.0f : 0.0f;
        GLfloat green = red == 0.0f && blue == 0.0f ? 1.0f : 0.0f;

        voxels[count] = renderer->createVoxel(glm::vec3(0.0f, 0.0f, 0.0f),                // Top - Left - Front position
                                              glm::vec3(voxelSize, voxelSize, voxelSize), // Size
                                              glm::vec3(red, green, blue));               // Color
    }

    glm::vec3 cameraPosition(0.0f, 0.0f, 0.0f);

    while (!window.isDone())
    {
        window.begin();

        glm::mat4 view       = glm::lookAt(cameraPosition, cameraPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::ortho(0.0f, 400.0f, 0.0f, 320.0f, -100.0f, 100.0f);

        glUseProgram(voxelShader);

        for (unsigned int count = 0; count < numVoxels; ++count)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(count * voxelSize, 160, 0));
            model = glm::rotate(model, (float)glm::radians(glfwGetTime() * 60), glm::vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 mvp = projection * view * model;

            renderer->passUniformMatrix(voxelShader, "mvp", mvp);
            renderer->drawArrays(voxels[count], 0, 36);
        }

        GLenum err = glGetError();
        while((err = glGetError()) != GL_NO_ERROR) LOG("OpenGL Error: " + std::to_string(int(err)));

        window.end();
    }

    delete ce::RendererLocator::getRenderer();
    delete ce::EventLocator::getEventQueue();

    return 0;
}