#pragma once
#include "Types.h"

namespace utils
{

class RepSocket
{
    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    RepSocket(types::ZmqContext& ctx, const std::string& addr, const std::string& port);
    //! Destructor.
    virtual ~RepSocket();

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
    //! Binds socket to the given address and port.
    void Bind(const std::string& addr, const std::string& port);

    //
    // Protected data members.
    //
protected:
    //! Zmq reply socket pointer.
    types::ZmqSocketPtr rep_;
    //! Zmq worker.
    types::ZmqWorker worker_;
    //! Is running flag.
    std::atomic<bool> isRunning_;
    //! Monitor pointer type.
    types::ZmqMonitorPtr monitor_;
};

} // namespace utils
