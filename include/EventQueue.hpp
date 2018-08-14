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

#ifndef CE_EVENT_QUEUE_HPP
#define CE_EVENT_QUEUE_HPP

////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////
#include <string>
#include <glm/glm.hpp>

#include "Logger.hpp"

namespace ce
{

#define MAX_EVENTS 16

struct Event
{
    Event(const std::string eventID)
        : id(eventID)
    { }

    Event()
    { }

    std::string id;

    union
    {
        bool      eventBool;
        int       eventInt;
        float     eventFloat;
        glm::vec2 eventVec;
    };
};

class IEventQueue
{
    public:
        virtual ~IEventQueue() { }
        virtual void addEvent(const Event & event) = 0;
        virtual bool processEvents(Event & event) = 0;
        virtual void clear() = 0;
};

// Implementing an event queue using a simple array as a ring buffer for performance.
class EventQueue : public IEventQueue
{
    public:
        EventQueue()
            : m_head(0), m_tail(0)
        { }

        void addEvent(const Event & event)
        {
            int newTail = (m_tail + 1) % MAX_EVENTS;

            if (newTail != m_head)
            {
                m_events[m_tail] = event;
                m_tail = newTail;
            }
            else
                LOG("Exceeded maximum events allowed in EventQueue.");
        }

        bool processEvents(Event & event)
        {
            if (m_head == m_tail)
                return false;
            
            event  = m_events[m_head];
            m_head = (m_head + 1) % MAX_EVENTS;
            
            return true;
        }

        void clear()
        {
            m_head = 0;
            m_tail = 0;
        }

    private:
        Event m_events[MAX_EVENTS];
        int m_head;
        int m_tail;
};

class NullEventQueue : public IEventQueue
{
    public:
        void addEvent(const Event & event) { }
        bool processEvents(Event & event) { return false; }
        void clear() { }
};

class EventLocator
{
    public:
        static void initialize()
        {
            m_service = &m_nullEventQueue;
        }

        static IEventQueue * getEventQueue()
        {
            return m_service;
        }

        static void provide(IEventQueue * service)
        {
            if (service == nullptr) m_service = &m_nullEventQueue;
            else                    m_service = service;
        }
    
    private:
        static IEventQueue *   m_service;
        static NullEventQueue  m_nullEventQueue;
};

} // namespace ce

#endif
