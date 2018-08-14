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
////////////////////////////////////////////////////////////////

#ifndef CE_LOGGER_HPP
#define CE_LOGGER_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#define LOGGER_CONTEXT_WIDTH 48
#define LOGGER_MESSAGE_BREAK_WIDTH 64

class Logger
{
    public:
        Logger(bool outputLive = false) : m_outputLive(outputLive), m_lastContextFile("")
        { }

        void log(std::string logMessage, std::string contextFile="", std::string contextMethod="", int contextLine=-1)
        {
            std::string logContext = "";

            if (contextLine > -1)
            {
                int pathEnd   = contextFile.find_last_of("/");
                std::string filename   = contextFile.substr(pathEnd != -1 ? (pathEnd + 1) : 0);
                std::string modulename = filename.substr(0, filename.length() - 4);

                if (modulename != m_lastContextFile)
                    logBreak(modulename);

                logContext = filename + "(" + std::to_string(contextLine) + ")::" + contextMethod + ":";
                m_lastContextFile = modulename;
            }

            m_logContexts.push_back(logContext);
            m_logMessages.push_back(logMessage);

            if (m_outputLive)
            {
                std::cout << std::left << std::setfill(' ') << std::setw(LOGGER_CONTEXT_WIDTH)
                          << logContext << logMessage << std::endl;
            }
        }

        void logBreak(std::string message)
        {
            log("");
            log(std::string(LOGGER_MESSAGE_BREAK_WIDTH, '-'));
        }

        friend std::ostream & operator<< (std::ostream & output, const Logger & logger)
        {
            int messageSideSize = (LOGGER_CONTEXT_WIDTH + LOGGER_MESSAGE_BREAK_WIDTH) / 2;
            output << std::right << std::setfill('-') << std::setw(messageSideSize + 8)
                   << "BEGIN LOG OUTPUT";
            output << std::setw(messageSideSize - 7) << '\n';

            for (unsigned int count = 0; count < logger.m_logMessages.size(); ++count)
            {
                output << std::left << std::setfill(' ') << std::setw(LOGGER_CONTEXT_WIDTH) 
                       << logger.m_logContexts[count] << logger.m_logMessages[count] << '\n';
            }

            output << '\n';
            output << std::right << std::setfill('-') << std::setw(messageSideSize + 7)
                   << "END LOG OUTPUT";
            output << std::setw(messageSideSize - 6) << '\n';

            return output;
        }

    private:
        /****************************************
         * Data members
        ****************************************/
        std::vector<std::string> m_logMessages;
        std::vector<std::string> m_logContexts;
        bool                     m_outputLive;
        std::string              m_lastContextFile;
};

// Create global logger
extern Logger globalLogger;

#define LOG(message) globalLogger.log(message, __FILE__, __func__, __LINE__)
#define LOGGER globalLogger

#define LOGGER_DECL Logger globalLogger;
#define LOGGER_DECL_LIVE Logger globalLogger(true);

#endif
