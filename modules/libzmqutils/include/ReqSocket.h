#pragma once
#include "Types.h"

namespace utils
{

class ReqSocket
{
    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    ReqSocket(utils::types::ZmqContext& ctx, const std::string& addr, const std::string& port);
    //! Destructor.
    virtual ~ReqSocket();

    //
    // Public types.
    //
public:
    //! Returns is running flag.
    bool IsRunning() const;

    //
    // Protected interface.
    //
protected:
    //! Processes messages.
    virtual void Process() = 0;
    //! Starts.
    virtual void Run();
    //! Stops.
    virtual void Stop();
    //! Connects socket to the given address and port.
    void Connect(const std::string& addr, const std::string& port);

    //
    // Protected data members.
    //
protected:
    //! Zmq request socket pointer.
    types::ZmqSocketPtr req_;
    //! Zmq worker.
    types::ZmqWorker worker_;
    //! Is running flag.
    std::atomic<bool> isRunning_;
    //! Monitor pointer type.
    types::ZmqMonitorPtr monitor_;
};

} // namespace utils
