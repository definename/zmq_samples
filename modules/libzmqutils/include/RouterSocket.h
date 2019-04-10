#pragma once
#include "Types.h"

namespace utils
{

class RouterSocket
{
    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    RouterSocket(
        utils::types::ZmqContext& ctx,
        const std::string& addr,
        const std::string& port,
        const std::string& identity);
    //! Destructor.
    virtual ~RouterSocket();

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
    //! Zmq router socket pointer.
    types::ZmqSocketPtr router_;
    //! Zmq worker.
    types::ZmqWorker worker_;
    //! Is running flag.
    std::atomic<bool> isRunning_;
    //! Monitor pointer type.
    types::ZmqMonitorPtr monitor_;
};

} // namespace utils
