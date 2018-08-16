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

#ifndef CE_RENDERER_SERVICE_HPP
#define CE_RENDERER_SERVICE_HPP

////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////
#include <vector>
#include <iostream>
#include "OpenGL.hpp"
#include <glm/glm.hpp>

#include "Logger.hpp"
#include "FileReader.hpp"

namespace ce
{

////////////////////////////////////////////////////////////////
// \brief Interface for the Renderer service, don't instantiate.
//
// \see Renderer
//
////////////////////////////////////////////////////////////////
class IRenderer
{
    public:
        virtual ~IRenderer() { }

        virtual GLuint generateVAO() = 0;
        virtual GLuint generateVBO() = 0;
        virtual GLuint generateTexture() = 0;
        virtual GLuint generateFrameBuffer() = 0;
        virtual void bindVAO(const GLuint & vao) = 0;
        virtual void bindArrayBuffer(const GLuint & vbo, const unsigned int & vertexDataSize, const GLvoid * data, const bool & staticDraw=true) = 0;
        virtual void bindTexture(const GLuint & texture) = 0;
        virtual void bindFrameBuffer(const GLuint & frameBuffer) = 0;
        virtual void addVertexAttribute(const GLint & size, const bool & normalize, const int & stride, const int & offset) = 0;
        virtual void loadTextureImage(const unsigned char * textureData, const unsigned int & width, const unsigned int & height) = 0;
        virtual void loadEmptyTextureImage(const unsigned int & width, const unsigned int & height) = 0;
        virtual void unbindVAO() = 0;
        virtual void unbindArrayBuffer() = 0;
        virtual void unbindTexture() = 0;
        virtual void drawArrays(const GLuint & vao, const int & first, const int & count) = 0;
        virtual void drawArraysTriangleFan(const GLuint & vao, const int & first, const int & count) = 0;
        virtual void setMinTextureFiltering(const GLint & filter) = 0;
        virtual void setMagTextureFiltering(const GLint & filter) = 0;
        virtual void setTextureWrapping(const GLint & wrapS, const GLint & wrapT) = 0;
        virtual void setTextureWrapping(const GLint & wrap) = 0;
        virtual GLuint createShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource) = 0;
        virtual GLuint createShaderProgramFromFiles(const char * vertexShaderFilename, const char * fragmentShaderFilename) = 0;
        virtual GLuint createRect(const GLfloat & width, const GLfloat & height, const glm::vec3 & color) = 0;
        virtual GLuint createRect(const glm::vec2 & tl, const glm::vec2 & br, const glm::vec3 & color) = 0;
        virtual GLuint createOctagon(const glm::vec2 & tl, const glm::vec2 & br, const glm::vec3 & color) = 0;
};

////////////////////////////////////////////////////////////////
// \brief Renderer service that provides both a wrapper around OpenGL
// calls, as well as some higher level methods for rendering.
//
////////////////////////////////////////////////////////////////
class Renderer : public IRenderer
{
    public:
        Renderer();
        ~Renderer();

        // Low level OpenGL wrapper methods
        GLuint generateVAO();
        GLuint generateVBO();
        GLuint generateTexture();
        GLuint generateFrameBuffer();

        void bindVAO(const GLuint & vao);
        void bindArrayBuffer(const GLuint & vbo, const unsigned int & vertexDataSize, const GLvoid * data, const bool & staticDraw=true);
        void bindTexture(const GLuint & texture);
        void bindFrameBuffer(const GLuint & framebuffer);
        void addVertexAttribute(const GLint & size, const bool & normalize, const int & stride, const int & offset);
        void loadTextureImage(const unsigned char * textureData, const unsigned int & width, const unsigned int & height);
        void loadEmptyTextureImage(const unsigned int & width, const unsigned int & height);

        void unbindVAO();
        void unbindArrayBuffer();
        void unbindTexture();

        void drawArrays(const GLuint & vao, const int & first, const int & count);
        void drawArraysTriangleFan(const GLuint & vao, const int & first, const int & count);

        void setMinTextureFiltering(const GLint & filter);
        void setMagTextureFiltering(const GLint & filter);

        void setTextureWrapping(const GLint & wrapS, const GLint & wrapT);
        void setTextureWrapping(const GLint & wrap);

        // High level methods
        GLuint createShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource);
        GLuint createShaderProgramFromFiles(const char * vertexShaderFilename, const char * fragmentShaderFilename);
        GLuint createRect(const GLfloat & width, const GLfloat & height, const glm::vec3 & color);
        GLuint createRect(const glm::vec2 & tl, const glm::vec2 & br, const glm::vec3 & color);
        GLuint createOctagon(const glm::vec2 & tl, const glm::vec2 & br, const glm::vec3 & color);

    private:
        std::vector<GLuint> m_vaoList;
        std::vector<GLuint> m_vboList;
        std::vector<GLuint> m_textureList;
        std::vector<GLuint> m_frameBufferList;

        unsigned int m_vertexAttributeCount;
};

class NullRenderer : public IRenderer
{
    public:
        GLuint generateVAO() { return 0; }
        GLuint generateVBO() { return 0; }
        GLuint generateTexture() { return 0; }
        GLuint generateFrameBuffer() { return 0; }
        void bindVAO(const GLuint & vao) { }
        void bindArrayBuffer(const GLuint & vbo, const unsigned int & vertexDataSize, const GLvoid * data, const bool & staticDraw=true) { }
        void bindTexture(const GLuint & texture) { }
        void bindFrameBuffer(const GLuint & framebuffer) { }
        void addVertexAttribute(const GLint & size, const bool & normalize, const int & stride, const int & offset) { }
        void loadTextureImage(const unsigned char * textureData, const unsigned int & width, const unsigned int & height) { }
        void loadEmptyTextureImage(const unsigned int & width, const unsigned int & height) { }
        void unbindVAO() { }
        void unbindArrayBuffer() { }
        void unbindTexture() { }
        void drawArrays(const GLuint & vao, const int & first, const int & count) { }
        void drawArraysTriangleFan(const GLuint & vao, const int & first, const int & count) { }
        void setMinTextureFiltering(const GLint & filter) { }
        void setMagTextureFiltering(const GLint & filter) { }
        void setTextureWrapping(const GLint & wrapS, const GLint & wrapT) { }
        void setTextureWrapping(const GLint & wrap) { }
        GLuint createShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource) { return 0; }
        GLuint createShaderProgramFromFiles(const char * vertexShaderFilename, const char * fragmentShaderFilename) { return 0; }
        GLuint createRect(const GLfloat & width, const GLfloat & height, const glm::vec3 & color) { return 0; }
        GLuint createRect(const glm::vec2 & tl, const glm::vec2 & br, const glm::vec3 & color) { return 0; }
        GLuint createOctagon(const glm::vec2 & tl, const glm::vec2 & br, const glm::vec3 & color) { return 0; }
};

////////////////////////////////////////////////////////////////
// \brief Provides a service locator to get the current global
// renderer service.
//
////////////////////////////////////////////////////////////////
class RendererLocator
{
    public:
        static void initialize()
        {
            // Initialize the renderer service to the null one to prevent any breakages
            // when getting the service before providing an instance.
            m_service = &m_nullRenderer;
        }

        static IRenderer * getRenderer()
        {
            return m_service;
        }

        static void provide(IRenderer * service)
        {
            if (service == nullptr) m_service = &m_nullRenderer;
            else                    m_service = service;
        }
    
    private:
        static IRenderer *  m_service;
        static NullRenderer m_nullRenderer;
};

} // namespace ce

#endif