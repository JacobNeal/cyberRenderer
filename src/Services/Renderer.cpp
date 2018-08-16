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

} // namespace ce