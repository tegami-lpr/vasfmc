#include "fmcmessagebus.h"
#include "eventqueue.h"
#include "logger.h"

//--------------------------------------------------------------------------------------------------------------------//

const std::string ANY_RECV = "*";

FMCMessageBus *FMCMessageBus::m_instance = nullptr;

//--------------------------------------------------------------------------------------------------------------------//

FMCMessage::FMCMessage(const std::string &sender, int32_t messageType) : FMCMessage (sender, messageType, ANY_RECV){
}

FMCMessage::FMCMessage(const std::string &sender, int32_t messageType, const std::string &receiver) {
    m_sender = sender;
    m_messageType = messageType;
    m_receiver = receiver;
}

FMCMessage::FMCMessage(const FMCMessage &other) {
    m_sender = other.m_sender;
    m_messageType = other.m_messageType;
    m_receiver = other.m_receiver;
}

//--------------------------------------------------------------------------------------------------------------------//

std::string FMCMessage::Sender() {
    return m_sender;
}

//--------------------------------------------------------------------------------------------------------------------//

std::string FMCMessage::Receiver() {
    return m_receiver;
}

//--------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------//

FMCMessageBus *FMCMessageBus::GetInstance() {
    if (m_instance == nullptr) {
        m_instance = new FMCMessageBus();
    }
    return m_instance;
}

//--------------------------------------------------------------------------------------------------------------------//

FMCMessageBus::FMCMessageBus() {
    qRegisterMetaType<FMCMessage>("FMCMessage");
    qRegisterMetaType<FMCMessage*>("FMCMessage*");
    m_loopThread = nullptr;
    m_eventQueue = new NPEventQueue<FMCMessage>();
}


FMCMessageBus::~FMCMessageBus() {
    if (m_loopThread) {
        m_breakThreadMutex.unlock();
        //generate dummy message to unlock queue wait lock
        m_eventQueue->AddEvent(new FMCMessage(ANY_RECV, -1));
    }
    delete m_eventQueue;
}

//--------------------------------------------------------------------------------------------------------------------//

void FMCMessageBus::Init() {
    //Taking mutex
    m_breakThreadMutex.lock();
    //Start thread loop
    m_loopThread = new std::thread(&FMCMessageBus::threadFunction, this);
    Logger::log("MessageBus inited");
}

//--------------------------------------------------------------------------------------------------------------------//

void FMCMessageBus::PutMessage(FMCMessage *message) {
    auto messageBus = GetInstance();
    messageBus->m_eventQueue->AddEvent(message);
}

//--------------------------------------------------------------------------------------------------------------------//

void FMCMessageBus::Subscribe(FMCMessageReceiver *subscriber, const std::string& id) {
    auto messageBus = GetInstance();

    std::lock_guard<std::mutex> lk(messageBus->m_subscribersMutex);
    for (const auto& item : messageBus->m_subscribers) {
        if (item.first == subscriber) return;
    }
    messageBus->m_subscribers.insert_after(messageBus->m_subscribers.end(), make_pair(subscriber, id));
}

//--------------------------------------------------------------------------------------------------------------------//

void FMCMessageBus::Unsubscribe(FMCMessageReceiver *subscriber) {
    auto messageBus = GetInstance();

    std::lock_guard<std::mutex> lk(messageBus->m_subscribersMutex);
    messageBus->m_subscribers.remove_if([subscriber](const std::pair<FMCMessageReceiver*, std::string>& pair) {return (pair.first == subscriber);});
}

//--------------------------------------------------------------------------------------------------------------------//

void FMCMessageBus::threadFunction() {
    std::forward_list<FMCMessageReceiver*> receivers;
    std::unique_lock<std::mutex> lck(m_subscribersMutex, std::defer_lock);
    do {
        if (m_breakThreadMutex.try_lock()) {
            //if we can take mutex, then we must finish loop
            return;
        }

        auto event = m_eventQueue->WaitEvent();
        do {
            if (event == nullptr) break;

            lck.lock();
            for (const auto& item : m_subscribers) {
                if (event->Receiver() == ANY_RECV) {
                    receivers.insert_after(receivers.end(), item.first);
                    continue;
                }
                if (event->Receiver() != item.second) continue;
                receivers.insert_after(receivers.end(), item.first);
            }
            lck.unlock();

            for (auto receiver : receivers) {
                //XXX: spawn many copy of message is bad design, but for start it ok...
                auto tmpMsg = new FMCMessage(*event);
                receiver->ReceiveMessage(tmpMsg);
                //QMetaObject::invokeMethod(receiver, "ReceiveMessage", Qt::QueuedConnection, Q_ARG(FMCMessage*, tmpMsg));
            }
            delete event;
            receivers.clear();
        } while(false);
        std::this_thread::yield();
    } while(true);
}
