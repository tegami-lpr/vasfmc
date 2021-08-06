#ifndef FMCMESSAGEBUS_H
#define FMCMESSAGEBUS_H

#include <forward_list>
#include <string>
#include <thread>
#include <mutex>

extern const std::string ANY_RECV;

template <class T> class NPEventQueue;

class FMCMessage {
public:
    FMCMessage() = default;
    FMCMessage(const std::string &sender, int32_t messageType);
    FMCMessage(const std::string &sender, int32_t messageType, const std::string &receiver);
    //! Copy constructor
    FMCMessage(const FMCMessage &other);

    std::string Sender();
    std::string Receiver();

    std::string m_sender;
    std::string m_receiver = ANY_RECV;
    int32_t m_messageType = -1;
};

class FMCMessageReceiver {
public:
    virtual void ReceiveMessage(FMCMessage* message);
};

//!Message bus for exchange data between components
class FMCMessageBus {
public:
    static FMCMessageBus* GetInstance();
    //!Destructor
    ~FMCMessageBus();
    //! Function for init messagebus loop
    void Init();

    //! Add message to the bus
    static void PutMessage(FMCMessage *message);
    //! Add new subscriber ot the bus
    static void Subscribe(FMCMessageReceiver *subscriber, const std::string& id);
    //! Remove new subscriber ot the bus
    static void Unsubscribe(FMCMessageReceiver *subscriber);

private:
    //! Constructor
    FMCMessageBus();
    //! Static pointer to message bus instance
    static FMCMessageBus *m_instance;
    //! Pointer to bus loop thread
    std::thread *m_loopThread;
    //! Mutex to stop loop
    std::mutex m_breakThreadMutex;
    //! Messages queue
    NPEventQueue<FMCMessage> *m_eventQueue;

    //! Mutex for handle subscribers access
    std::mutex m_subscribersMutex;
    std::forward_list<std::pair<FMCMessageReceiver*, std::string>> m_subscribers;

    //! Function for message bus loop
    void threadFunction();
};


#endif //FMCMESSAGEBUS_H
