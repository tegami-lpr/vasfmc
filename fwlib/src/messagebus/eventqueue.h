#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

template <class T>
class NPEventQueue {
public:
    ~NPEventQueue() {
        while (!m_eventQueue.empty()) {
            auto event = m_eventQueue.front();
            m_eventQueue.pop();
            delete event;
        }
    }
    void AddEvent(T* event) {
        {
            //Taking mutex and put event to queue
            std::lock_guard<std::mutex> lk(m_mutex);
            m_eventQueue.push(event);
        }
        //Inform waiting thread about new  event in queue
        m_cv.notify_one();
    }
    T* WaitEvent() {
        //Taking mutex
        std::unique_lock<std::mutex> lk(m_mutex);
        //and starting wait for new event in queue
        m_cv.wait(lk, [this]{return !m_eventQueue.empty();});
        //after new item in queue we can take it from queue
        auto event = m_eventQueue.front();
        m_eventQueue.pop();
        //releasing mutex
        lk.unlock();
        //and return event
        return event;
    }
private:
    std::queue<T*> m_eventQueue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};


#endif //EVENTQUEUE_H
