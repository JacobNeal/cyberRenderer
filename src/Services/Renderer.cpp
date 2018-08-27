//////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////

#include "Services/Renderer.hpp"

ce::IRenderer *  ce::RendererLocator::m_service = nullptr;
ce::NullRenderer ce::RendererLocator::m_nullRenderer;

namespace ce
{

//////////////////////////////////////////////////////////////
Renderer::Renderer() : m_vertexAttributeCount(0)
{ }

//////////////////////////////////////////////////////////////
Renderer::~Renderer()
{
    for (GLuint & vao : m_vaoList)
        glDeleteVertexArrays(1, &vao);

    for (GLuint & vbo : m_vboList)
        glDeleteBuffers(1, &vbo);

    for (GLuint & texture : m_textureList)
        glDeleteTextures(1, &texture);

    for (GLuint & frameBuffer : m_frameBufferList)
        glDeleteFramebuffers(1, &frameBuffer);

    m_vaoList.clear();
    m_vboList.clear();
    m_textureList.clear();
    m_frameBufferList.clear();
}

//////////////////////////////////////////////////////////////
GLuint Renderer::generateVAO()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    m_vaoList.push_back(vao);

    return vao;
}

//////////////////////////////////////////////////////////////
GLuint Renderer::generateVBO()
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    m_vboList.push_back(vbo);

    return vbo;
}

//////////////////////////////////////////////////////////////
GLuint Renderer::generateTexture()
{
    GLuint texture;
    glGenTextures(1, &texture);
    m_textureList.push_back(texture);

    return texture;
}

//////////////////////////////////////////////////////////////
GLuint Renderer::generateFrameBuffer()
{
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    m_frameBufferList.push_back(framebuffer);

    return framebuffer;
}

//////////////////////////////////////////////////////////////
GLuint Renderer::generateRenderBuffer()
{
    GLuint renderBuffer;
    glGenRenderbuffers(1, &renderBuffer);
    m_renderBufferList.push_back(renderBuffer);

    return renderBuffer;
}

//////////////////////////////////////////////////////////////
void Renderer::bindVAO(const GLuint & vao)
{
    glBindVertexArray(vao);
    m_vertexAttributeCount = 0;
}

//////////////////////////////////////////////////////////////
void Renderer::bindArrayBuffer(const GLuint & vbo, const unsigned int & vertexDataSize, const GLvoid * data, const bool & staticDraw)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexDataSize, data, staticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    m_vertexAttributeCount = 0;
}

//////////////////////////////////////////////////////////////
void Renderer::bindTexture(const GLuint & texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

//////////////////////////////////////////////////////////////
void Renderer::bindFrameBuffer(const GLuint & framebuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

//////////////////////////////////////////////////////////////
void Renderer::bindRenderBuffer(const GLuint & renderBuffer)
{
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
}

//////////////////////////////////////////////////////////////
void Renderer::addVertexAttribute(const GLint & size, const bool & normalize, const int & stride, const int & offset)
{
    glVertexAttribPointer(m_vertexAttributeCount, size, GL_FLOAT, normalize ? GL_TRUE : GL_FALSE, stride, (GLvoid *)offset);
    glEnableVertexAttribArray(m_vertexAttributeCount);
    ++m_vertexAttributeCount;
}

//////////////////////////////////////////////////////////////
void Renderer::loadTextureImage(const unsigned char * textureData, const unsigned int & width, const unsigned int & height)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &textureData[0]);
}

//////////////////////////////////////////////////////////////
void Renderer::loadEmptyTextureImage(const unsigned int & width, const unsigned int & height)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
}

//////////////////////////////////////////////////////////////
void Renderer::attachRenderBufferToFrameBuffer(const GLuint & renderBuffer, const GLenum & renderBufferTarget)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderBufferTarget, GL_RENDERBUFFER, renderBuffer);
}

//////////////////////////////////////////////////////////////
void Renderer::unbindVAO()
{
    glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////
void Renderer::unbindArrayBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////////
void Renderer::unbindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

//////////////////////////////////////////////////////////////
void Renderer::setActiveTexture(const GLuint & textureHandle, const GLenum & texture)
{
    glActiveTexture(texture);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
}

//////////////////////////////////////////////////////////////
void Renderer::setTextureSampler(const GLuint & shaderProgram, const char * uniformName)
{
    GLint uniformLocation = glGetUniformLocation(shaderProgram, uniformName);
    glUniform1i(uniformLocation, 0);
}

//////////////////////////////////////////////////////////////
void Renderer::setRenderBufferStorage(const GLsizei & width, const GLsizei & height, const GLenum & internalFormat)
{
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}

//////////////////////////////////////////////////////////////
void Renderer::setFrameBufferTexture(const GLuint & textureHandle, const GLenum & attachment, const GLint & mipMapLevel)
{
    glFramebufferTexture(GL_FRAMEBUFFER, attachment, textureHandle, mipMapLevel);
}

//////////////////////////////////////////////////////////////
void Renderer::drawArrays(const GLuint & vao, const int & first, const int & count)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, first, count);
    glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////
void Renderer::drawArraysTriangleFan(const GLuint & vao, const int & first, const int & count)
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, first, count);
    glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////
void Renderer::setColorDrawBuffer()
{
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG("A problem occurred while setting up the framebuffer.");
}

//////////////////////////////////////////////////////////////
void Renderer::setMinTextureFiltering(const GLint & filter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}

//////////////////////////////////////////////////////////////
void Renderer::setMagTextureFiltering(const GLint & filter)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

//////////////////////////////////////////////////////////////
void Renderer::setTextureWrapping(const GLint & wrapS, const GLint & wrapT)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}

//////////////////////////////////////////////////////////////
void Renderer::setTextureWrapping(const GLint & wrap)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
}

//////////////////////////////////////////////////////////////
void Renderer::passUniformMatrix(const GLuint & shaderProgram, const char * uniformName, const glm::mat4 & uniformMatrix, const bool & normalize)
{
    GLint uniformLocation = glGetUniformLocation(shaderProgram, uniformName);
    glUniformMatrix4fv(uniformLocation, 1, normalize ? GL_TRUE : GL_FALSE, glm::value_ptr(uniformMatrix));
}

//////////////////////////////////////////////////////////////
GLuint Renderer::createShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource)
{
    GLuint program;

    GLuint vertexShader, fragmentShader;
    GLint success;
    GLchar infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Report any errors that occurred during compilation
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOG("ERROR: Failed to compile the vertex shader");
        LOG(std::string(infoLog));
    }
    else
        LOG("Compiled vertex shader...");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Report any errors that occurred during compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOG("ERROR: Failed to compile the fragment shader");
        LOG(std::string(infoLog));
    }
    else
        LOG("Compiled fragment shader...");

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Report any errors that occurred during linking
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        LOG("ERROR: Failed to link shaders into shader program");
        LOG(std::string(infoLog));
    }
    else
        LOG("Linked shaders into shader program.");

    // Delete the shaders now that they have been linked to free memory
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

//////////////////////////////////////////////////////////////
GLuint Renderer::createShaderProgramFromFiles(const char * vertexShaderFilename, const char * fragmentShaderFilename)
{
    GLuint program;

    FileReader<char> shaderReader;
    char * vertexShaderSource;
    size_t vertexShaderSourceSize;

    char * fragmentShaderSource;
    size_t fragmentShaderSourceSize;

    if (shaderReader.read(vertexShaderFilename, &vertexShaderSource, &vertexShaderSourceSize) &&
        shaderReader.read(fragmentShaderFilename, &fragmentShaderSource, &fragmentShaderSourceSize))
    {
        program = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    }

    delete [] vertexShaderSource;
    delete [] fragmentShaderSource;

    return program;
}

//////////////////////////////////////////////////////////////
GLuint Renderer::createRect(const GLfloat & width, const GLfloat & height, const glm::vec3 & color)
{
    const GLfloat vertexData[] = {
        0.0f,  0.0f,   0.0f, color.r, color.g, color.b,  // BL
        width, 0.0f,   0.0f, color.r, color.g, color.b,  // BR
        width, height, 0.0f, color.r, color.g, color.b,  // TR
        0.0f,  0.0f,   0.0f, color.r, color.g, color.b,  // BL
        width, height, 0.0f, color.r, color.g, color.b,  // TR
        0.0f,  height, 0.0f, color.r, color.g, color.b   // TL
    };

    GLuint vao = generateVAO();
    GLuint vbo = generateVBO();

    bindVAO(vao);
    bindArrayBuffer(vbo, sizeof(vertexData), &vertexData[0]);

    addVertexAttribute(3, false, 6 * sizeof(GLfloat), 0);
    addVertexAttribute(3, false, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    unbindVAO();

    m_vaoList.push_back(vao);
    m_vboList.push_back(vbo);

    return vao;
}

//////////////////////////////////////////////////////////////
GLuint Renderer::createRect(const glm::vec2 & tl, const glm::vec2 & br, const glm::vec3 & color)
{
    const GLfloat vertexData[] = {
        tl.x, br.y, 0.0f, color.r, color.g, color.b, // BL
        br.x, br.y, 0.0f, color.r, color.g, color.b, // BR
        br.x, tl.y, 0.0f, color.r, color.g, color.b, // TR
        tl.x, br.y, 0.0f, color.r, color.g, color.b, // BL
        br.x, tl.y, 0.0f, color.r, color.g, color.b, // TR
        tl.x, tl.y, 0.0f, color.r, color.g, color.b  // TL
    };

    GLuint vao = generateVAO();
    GLuint vbo = generateVBO();

    bindVAO(vao);
    bindArrayBuffer(vbo, sizeof(vertexData), &vertexData[0]);

    addVertexAttribute(3, false, 6 * sizeof(GLfloat), 0);
    addVertexAttribute(3, false, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    unbindVAO();

    m_vaoList.push_back(vao);
    m_vboList.push_back(vbo);

    return vao;
}

//////////////////////////////////////////////////////////////
GLuint Renderer::createOctagon(const glm::vec2 & tl, const glm::vec2 & br, const glm::vec3 & color)
{
    GLfloat width  = br.x - tl.x;
    GLfloat height = tl.y - br.y;

    GLfloat x_1_8 = tl.x + (0.125f * width); // 1 / 8 = 0.125f
    GLfloat x_7_8 = br.x - (0.125f * width);
    GLfloat y_1_4 = tl.y - (0.25f * height); // 1 / 4 = 0.25f
    GLfloat y_3_4 = br.y + (0.25f * height);

    const GLfloat vertexData[] = {
        tl.x + (width / 2), tl.y - (height / 2), 0.0f, color.r, color.g, color.b, // Center
        x_7_8, tl.y,  0.0f, color.r, color.g, color.b,
        x_1_8, tl.y,  0.0f, color.r, color.g, color.b,
        tl.x,  y_1_4, 0.0f, color.r, color.g, color.b,
        tl.x,  y_3_4, 0.0f, color.r, color.g, color.b,
        x_1_8, br.y,  0.0f, color.r, color.g, color.b,
        x_7_8, br.y,  0.0f, color.r, color.g, color.b,
        br.x,  y_3_4, 0.0f, color.r, color.g, color.b,
        br.x,  y_1_4, 0.0f, color.r, color.g, color.b,
        x_7_8, tl.y,  0.0f, color.r, color.g, color.b
    };

    GLuint vao = generateVAO();
    GLuint vbo = generateVBO();

    bindVAO(vao);
    bindArrayBuffer(vbo, sizeof(vertexData), &vertexData[0]);

    addVertexAttribute(3, false, 6 * sizeof(GLfloat), 0);
    addVertexAttribute(3, false, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    unbindVAO();

    m_vaoList.push_back(vao);
    m_vboList.push_back(vbo);

    return vao;
}

//////////////////////////////////////////////////////////////
GLuint Renderer::createVoxel(const glm::vec3 & tl, const glm::vec3 & size, const glm::vec3 & color)
{
    // Voxel vertex data size:
    //
    // 9 floats * 3 vectors * 2 triangles * 6 sides = 324 floats
    const GLfloat voxelData[] = {
        // Back side
        tl.x,          tl.y,          tl.z - size.z, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f,
        tl.x,          tl.y - size.y, tl.z - size.z, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f,
        tl.x + size.x, tl.y - size.y, tl.z - size.z, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f,
        tl.x + size.x, tl.y,          tl.z - size.z, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f,
        tl.x,          tl.y,          tl.z - size.z, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f,
        tl.x + size.x, tl.y - size.y, tl.z - size.z, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f,

        // Front side
        tl.x,          tl.y,          tl.z, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
        tl.x,          tl.y - size.y, tl.z, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
        tl.x + size.x, tl.y - size.y, tl.z, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
        tl.x + size.x, tl.y,          tl.z, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
        tl.x,          tl.y,          tl.z, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,
        tl.x + size.x, tl.y - size.y, tl.z, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f,

        // Left side
        tl.x, tl.y,          tl.z,          color.r, color.g, color.b, -1.0f, 0.0f, 0.0f,
        tl.x, tl.y - size.y, tl.z,          color.r, color.g, color.b, -1.0f, 0.0f, 0.0f,
        tl.x, tl.y - size.y, tl.z - size.z, color.r, color.g, color.b, -1.0f, 0.0f, 0.0f,
        tl.x, tl.y,          tl.z - size.z, color.r, color.g, color.b, -1.0f, 0.0f, 0.0f,
        tl.x, tl.y,          tl.z,          color.r, color.g, color.b, -1.0f, 0.0f, 0.0f,
        tl.x, tl.y - size.y, tl.z - size.z, color.r, color.g, color.b, -1.0f, 0.0f, 0.0f,

        // Right side
        tl.x + size.x, tl.y,          tl.z,          color.r, color.g, color.b, 1.0f, 0.0f, 0.0f,
        tl.x + size.x, tl.y - size.y, tl.z,          color.r, color.g, color.b, 1.0f, 0.0f, 0.0f,
        tl.x + size.x, tl.y - size.y, tl.z - size.z, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f,
        tl.x + size.x, tl.y,          tl.z - size.z, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f,
        tl.x + size.x, tl.y,          tl.z,          color.r, color.g, color.b, 1.0f, 0.0f, 0.0f,
        tl.x + size.x, tl.y - size.y, tl.z - size.z, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f,

        // Top side
        tl.x,          tl.y, tl.z,          color.r, color.g, color.b, 0.0f, 1.0f, 0.0f,
        tl.x,          tl.y, tl.z - size.z, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f,
        tl.x + size.x, tl.y, tl.z - size.z, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f,
        tl.x + size.x, tl.y, tl.z,          color.r, color.g, color.b, 0.0f, 1.0f, 0.0f,
        tl.x,          tl.y, tl.z,          color.r, color.g, color.b, 0.0f, 1.0f, 0.0f,
        tl.x + size.x, tl.y, tl.z - size.z, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f,

        // Bottom side
        tl.x,          tl.y - size.y, tl.z,          color.r, color.g, color.b, 0.0f, -1.0f, 0.0f,
        tl.x,          tl.y - size.y, tl.z - size.z, color.r, color.g, color.b, 0.0f, -1.0f, 0.0f,
        tl.x + size.x, tl.y - size.y, tl.z - size.z, color.r, color.g, color.b, 0.0f, -1.0f, 0.0f,
        tl.x + size.x, tl.y - size.y, tl.z,          color.r, color.g, color.b, 0.0f, -1.0f, 0.0f,
        tl.x,          tl.y - size.y, tl.z,          color.r, color.g, color.b, 0.0f, -1.0f, 0.0f,
        tl.x + size.x, tl.y - size.y, tl.z - size.z, color.r, color.g, color.b, 0.0f, -1.0f, 0.0f,
    };

    GLuint vao = generateVAO();
    GLuint vbo = generateVBO();

    bindVAO(vao);
    bindArrayBuffer(vbo, sizeof(voxelData), &voxelData[0]);

    addVertexAttribute(3, false, 9 * sizeof(GLfloat), 0);
    addVertexAttribute(3, false, 9 * sizeof(GLfloat), 3 * sizeof(GLfloat));
    addVertexAttribute(3, false, 9 * sizeof(GLfloat), 6 * sizeof(GLfloat));

    unbindVAO();

    m_vaoList.push_back(vao);
    m_vboList.push_back(vbo);

    return vao;
}

//////////////////////////////////////////////////////////////
GLuint Renderer::createMesh(const std::string & filename, GLuint & texture, size_t & numVertices)
{
    FileReader<char> fileReader;
    char * meshData;
    size_t meshSize;

    std::vector<glm::vec3> vertexCoordinates;
    std::vector<glm::vec3> normalCoordinates;
    std::vector<glm::vec3> textureCoordinates;
    std::string materialFilename = "";
    std::string textureFilename = "";
    std::vector<GLfloat> vertexData;

    GLuint vao, vbo;

    if (fileReader.read(filename.c_str(), &meshData, &meshSize))
    {
        char * buffer = strtok(meshData, " ,/\n\0");
        while (buffer != NULL)
        {
            glm::vec3 coord;
            if (strcmp("v", buffer) == 0)
            {
                buffer = strtok(NULL, " ,/\n\0");
                coord.x = atof(buffer);

                buffer = strtok(NULL, " ,/\n\0");
                coord.y = atof(buffer);

                buffer = strtok(NULL, " ,/\n\0");
                coord.z = atof(buffer);

                vertexCoordinates.push_back(coord);
            }
            else if (strcmp("vt", buffer) == 0)
            {
                buffer = strtok(NULL, " ,/\n\0");
                coord.x = atof(buffer);

                buffer = strtok(NULL, " ,/\n\0");
                coord.y = atof(buffer);

                textureCoordinates.push_back(coord);
            }
            else if (strcmp("vn", buffer) == 0)
            {
                buffer = strtok(NULL, " ,/\n\0");
                coord.x = atof(buffer);

                buffer = strtok(NULL, " ,/\n\0");
                coord.y = atof(buffer);

                buffer = strtok(NULL, " ,/\n\0");
                coord.z = atof(buffer);

                normalCoordinates.push_back(coord);
            }
            else if (strcmp("f", buffer) == 0)
            {
                for (unsigned int vertexCount = 0; vertexCount < 3; ++vertexCount)
                {
                    buffer = strtok(NULL, " ,/\n\0");
                    int vertexIndex  = atoi(buffer) - 1;

                    buffer = strtok(NULL, " ,/\n\0");
                    int textureIndex = atoi(buffer) - 1;

                    buffer = strtok(NULL, " ,/\n\0");
                    int normalIndex  = atoi(buffer) - 1;

                    vertexData.push_back(vertexCoordinates[vertexIndex].x);
                    vertexData.push_back(vertexCoordinates[vertexIndex].y);
                    vertexData.push_back(vertexCoordinates[vertexIndex].z);
                    
                    vertexData.push_back(textureCoordinates[textureIndex].x);
                    vertexData.push_back(textureCoordinates[textureIndex].y);
                    
                    vertexData.push_back(normalCoordinates[normalIndex].x);
                    vertexData.push_back(normalCoordinates[normalIndex].y);
                    vertexData.push_back(normalCoordinates[normalIndex].z);
                }
            }
            else if (strcmp("mtllib", buffer) == 0)
            {
                buffer = strtok(NULL, " ,/\n\0");
                materialFilename = std::string(buffer);
            }
            buffer = strtok(NULL, " ,/\n\0");
        }

        delete [] meshData;
    }

    if (materialFilename != "")
    {
        char * materialData;
        size_t materialDataSize;
        std::size_t endOfPath = filename.find_last_of("/") + 1;
        std::string pathToModel = filename.substr(0, endOfPath);
        materialFilename = pathToModel + materialFilename;

        if (fileReader.read(materialFilename.c_str(), &materialData, &materialDataSize))
        {
            char * buffer = strtok(materialData, " ,/\n\0");
            while (buffer != NULL)
            {
                if (strcmp("map_Kd", buffer) == 0)
                {
                    buffer = strtok(NULL, " ,/\n\0");
                    std::string textureFilename(buffer);

                    //////////////////////////////////////////
                    // Load the texture PNG file
                    //////////////////////////////////////////
                    ce::Image image(pathToModel + textureFilename);

                    texture = generateTexture();

                    bindTexture(texture);
                    loadTextureImage(&image.getImageBuffer()[0], image.getWidth(), image.getHeight());

                    setTextureWrapping(GL_REPEAT);
                    setMinTextureFiltering(GL_NEAREST);
                    setMagTextureFiltering(GL_NEAREST);

                    unbindTexture();
                }

                buffer = strtok(NULL, " ,/\n\0");
            }

            delete [] materialData;
        }
    }

    size_t vertexDataSize = vertexData.size();
    if (vertexDataSize > 0)
    {
        numVertices = vertexDataSize / 8;
        vao = generateVAO();
        vbo = generateVBO();

        bindVAO(vao);
        bindArrayBuffer(vbo, vertexDataSize * sizeof(GLfloat), &vertexData[0]);

        GLuint stride = 8 * sizeof(GLfloat);
        addVertexAttribute(3, false, stride, 0);
        addVertexAttribute(3, false, stride, 3 * sizeof(GLfloat));
        addVertexAttribute(3, false, stride, 5 * sizeof(GLfloat));

        unbindArrayBuffer();
        unbindVAO();
    }

    return vao;
}

} // namespace ce