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

#ifndef CE_IMAGE_HPP
#define CE_IMAGE_HPP

////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////
#include <vector>
#include <fstream>

#include "picoPNG.hpp"
#include "Logger.hpp"

namespace ce
{

////////////////////////////////////////////////////////////////
// \brief Loads a PNG image into a vector that can be used
// for texture mapping.
//
////////////////////////////////////////////////////////////////
class Image
{
    public:
        ////////////////////////////////////////////////////////////////
        Image()
        { }

        ////////////////////////////////////////////////////////////////
        Image(const std::string & filename)
        {
            loadFromFile(filename);
        }

        ////////////////////////////////////////////////////////////////
        void loadFromFile(const std::string & filename)
        {
            LOG("Reading image: " + filename);
            std::vector<unsigned char> buffer;

            std::ifstream file(filename.c_str(), 
                std::ios::in|std::ios::binary|std::ios::ate);

            ///////////////////////////////
            // Get the filesize
            ///////////////////////////////
            std::streamsize size = 0;
            if(file.seekg(0, std::ios::end).good()) 
                size = file.tellg();
            if (file.seekg(0, std::ios::beg).good())
                size -= file.tellg();

            //////////////////////////////////
            // Read the contents of the file
            //////////////////////////////////
            if (size > 0)
            {
                buffer.resize((size_t)size);
                file.read((char*)(&buffer[0]), size);
            }
            else
                buffer.clear();

            int error = decodePNG(m_image, m_width, m_height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size());

            if (error != 0)
                LOG("Error occurred while decoding the PNG. (error: " + std::to_string(error) + ")");
            else
            {
                LOG("Image size: " + std::to_string(m_width) + ", "
                                   + std::to_string(m_height));
            }
        }

        ////////////////////////////////////////////////////////////////
        std::vector<unsigned char> & getImageBuffer()
        {
            return m_image;
        }

        unsigned long getWidth()
        {
            return m_width;
        }

        unsigned long getHeight()
        {
            return m_height;
        }

    private:
        ////////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////////
        std::vector<unsigned char> m_image;
        
        unsigned long m_width;
        unsigned long m_height;
};

} // namespace ce

#endif
