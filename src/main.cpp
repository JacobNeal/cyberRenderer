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

    size_t meshVertexCount = 0;
    GLuint meshTexture = 0;
    GLuint meshVAO = renderer->createMesh("../resources/models/blacksmith/blacksmith.obj", meshTexture, meshVertexCount);
    GLuint meshShader = renderer->createShaderProgramFromFiles("../resources/shaders/entity_textured/vertex.glsl", "../resources/shaders/entity_textured/fragment.glsl");

    size_t nanosuitVertexCount = 0;
    GLuint nanosuitTexture = 0;
    GLuint nanosuitVAO = renderer->createMesh("../resources/models/nanosuit/nanosuit.obj", nanosuitTexture, nanosuitVertexCount);

    GLuint quadVAO = 0;
    GLuint renderedTexture = 0;
    GLuint frameBuffer = renderer->createFrameBuffer(800, 640, renderedTexture, quadVAO);
    GLuint quadShader = renderer->createShaderProgramFromFiles("../resources/shaders/texture/vertex.glsl", "../resources/shaders/texture/fragment.glsl");

    glm::vec3 cameraPosition(0.0f, 0.0f, 0.0f);

    while (!window.isDone())
    {
        window.begin();

        glm::mat4 view       = glm::lookAt(cameraPosition, cameraPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::ortho(0.0f, 400.0f, 0.0f, 320.0f, -100.0f, 100.0f);

        // Render to the framebuffer
        renderer->bindFrameBuffer(frameBuffer);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

        glUseProgram(meshShader);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(200.0f, 240.0f, 0.0f));
        model = glm::rotate(model, (float)glm::radians(glfwGetTime() * 60), glm::vec3(1.0f, 1.0f, 1.0f));

        renderer->passUniformMatrix(meshShader, "model", model);
        renderer->passUniformMatrix(meshShader, "view", view);
        renderer->passUniformMatrix(meshShader, "projection", projection);

        renderer->setActiveTexture(meshTexture);
        renderer->setTextureSampler(meshShader, "text");

        renderer->drawArrays(meshVAO, 0, meshVertexCount);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(200.0f, 40.0f, 0.0f));
        model = glm::rotate(model, (float)glm::radians(glfwGetTime() * 60), glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

        renderer->passUniformMatrix(meshShader, "model", model);
        renderer->drawArrays(nanosuitVAO, 0, nanosuitVertexCount);

        // Render to the screen
        renderer->bindFrameBuffer(0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(quadShader);

        renderer->setActiveTexture(renderedTexture);
        renderer->setTextureSampler(quadShader, "text");

        renderer->drawArrays(quadVAO, 0, 6);

        renderer->bindFrameBuffer(0);
        renderer->unbindVAO();

        GLenum err = glGetError();
        while((err = glGetError()) != GL_NO_ERROR) LOG("OpenGL Error: " + std::to_string(int(err)));

        window.end();
    }

    delete ce::RendererLocator::getRenderer();
    delete ce::EventLocator::getEventQueue();

    return 0;
}