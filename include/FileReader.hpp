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

#ifndef CE_FILE_READER_HPP
#define CE_FILE_READER_HPP

//////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////
#include <cstdio>
#include <cstdlib>

#include "Logger.hpp"

#define CE_FILEREADER_CHUNK_SIZE 32768       // 128 x 256
#define CE_FILEREADER_CHAR_CHUNK_SIZE 262144 // 256 x 1024

namespace ce
{

template <typename T>
class FileReader
{
    public:
        bool read(const char * filename, T ** dataPtr, size_t * sizePtr);
};

template <typename T>
bool FileReader<T>::read(const char * filename, T ** dataPtr, size_t * sizePtr)
{
    if (filename == nullptr || dataPtr == nullptr || sizePtr == nullptr)
    {
        LOG("No parameters can be null.");
        return false;
    }

    FILE * file = fopen(filename, "r");

    if (file != nullptr)
    {
        T * data = nullptr;
        T * temp;

        size_t size = 0;
        size_t used = 0;
        size_t numRead = -1;

        while (numRead != 0)
        {
            if (used + (CE_FILEREADER_CHUNK_SIZE * sizeof(T)) + sizeof(T) > size)
            {
                size = used + (CE_FILEREADER_CHUNK_SIZE * sizeof(T)) + sizeof(T);
                temp = (T *) realloc(data, size);

                if (temp == nullptr)
                {
                    free(data);
                    LOG("No memory left to complete this file operation.");
                    return false;
                }

                data = temp;
            }

            numRead = fread(data + used, sizeof(T), CE_FILEREADER_CHUNK_SIZE, file);
            used += numRead;
        }

        temp = (T *) realloc(data, used * sizeof(T));
        data = temp;

        *dataPtr = data;
        *sizePtr = used;

        fclose(file);
    }
    else
    {
        LOG("Could not open the file: " + std::string(filename));
        return false;
    }
    
    return true;
}

template <>
inline bool FileReader<char>::read(const char * filename, char ** dataPtr, size_t * sizePtr)
{
    if (filename == nullptr || dataPtr == nullptr || sizePtr == nullptr)
    {
        LOG("No parameters can be null.");
        return false;
    }

    FILE * file = fopen(filename, "r");

    if (file != nullptr)
    {
        char * data = nullptr;
        char * temp;

        size_t size = 0;
        size_t used = 0;
        size_t numRead = -1;

        while (numRead != 0)
        {
            if (used + CE_FILEREADER_CHAR_CHUNK_SIZE + 1 > size)
            {
                size = used + CE_FILEREADER_CHAR_CHUNK_SIZE + 1;
                temp = (char *) realloc(data, size);

                if (temp == nullptr)
                {
                    free(data);
                    LOG("No memory left to complete this file operation.");
                    return false;
                }

                data = temp;
            }

            numRead = fread(data + used, 1, CE_FILEREADER_CHAR_CHUNK_SIZE, file);
            used += numRead;
        }

        temp = (char *) realloc(data, used + 1);
        if (temp == nullptr)
        {
            free(data);
            LOG("No memory left to complete this file operation.");
            return false;
        }

        data = temp;
        data[used] = '\0';

        *dataPtr = data;
        *sizePtr = used;

        fclose(file);
    }
    else
    {
        LOG("Could not open the file: " + std::string(filename));
        return false;
    }
    
    return true;
}

} // namespace ce

#endif