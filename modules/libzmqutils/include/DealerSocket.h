#pragma once
#include "Types.h"

namespace utils
{

class DealerSocket
{
    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    DealerSocket(
        utils::types::ZmqContext& ctx,
        const std::string& addr,
        const std::string& port,
        const std::string& identity);
    //! Destructor.
    virtual ~DealerSocket();

    //
    // Public types.
    //
public:
    //! Connects socket to the given address and port.
    void Connect(const std::string& addr, const std::string& port);
    //! Returns is running flag.
    bool IsRunning() const;
    //! Starts socket.
    virtual void Run();
    //! Stops socket.
    virtual void Stop();

    //
    // Protected interface.
    //
protected:
    //! Receives first available message list, returns true if message was actually received.
    virtual bool Receive(utils::types::ZmqMessageList& msgList);
    //! Sends message, by adding message to the send queue.
    virtual void Send(const utils::types::ZmqMessageList& msgList);

    //
    // Protected data members.
    //
protected:
    //! Zmq dealer socket pointer.
    types::ZmqSocketPtr dealer_;
    //! Monitor pointer type.
    types::ZmqMonitorPtr monitor_;
    //! Receive queue.
    moodycamel::BlockingConcurrentQueue<utils::types::ZmqMessageList> recvQueue_;
    //! Send queue.
    moodycamel::BlockingConcurrentQueue<utils::types::ZmqMessageList> sendQueue_;

    //
    // Private interface.
    //
private:
    //! Actually sends message.
    void DoSend();
    //! Actually receives message.
    void DoReceive();

    //
    // Private data members.
    //
private:
    //! Is running flag.
    std::atomic<bool> isRunning_;
    //! Receiving thread.
    types::ZmqWorker recvThread_;
    //! Sending thread.
    types::ZmqWorker sendThread_;
};

} // namespace utils
