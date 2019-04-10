#pragma once
#include "Types.h"

namespace utils
{

class SubSocket
{
    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    SubSocket(types::ZmqContext& ctx, const std::string& addr, const std::string& port);
    //! Destructor.
    ~SubSocket();

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
    //! Returns is running flag.
    bool IsRunning() const;
    //! Connects socket to the given address and port.
    void Connect(const std::string& addr, const std::string& port);

    //
    // Protected data members.
    //
protected:
    //! Zmq pub socket pointer.
    types::ZmqSocketPtr sub_;
    //! Zmq worker.
    types::ZmqWorker worker_;
    //! Is running flag.
    std::atomic<bool> isRunning_;
    //! Monitor pointer type.
    types::ZmqMonitorPtr monitor_;
};

} // namespace utils
