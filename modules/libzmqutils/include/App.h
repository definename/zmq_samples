#pragma once

namespace utils
{

class App
{
    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    App()
        : terminated_(false)
    { }
    //! Destructor.
    virtual ~App()
    {
        Terminate();
    }

    //
    // Public interface
    //
public:
    //! Terminate.
    virtual void Terminate()
    {
		if (!terminated_)
		{
			std::unique_lock<std::mutex> l(mutex_);
			terminated_ = true;
			cv_.notify_all();
		}
    }
    //! Waits for termination request.
    void WaitForTerminationRequest()
    {
        std::unique_lock<std::mutex> l(mutex_);
		while (!terminated_) {
			cv_.wait(l);
		}
    }

    //
    // Private data members.
    //
private:
    //! App condition variable.
    std::condition_variable_any cv_;
    //! Synchronization mutex.
    std::mutex mutex_;
    //! Termination flag.
    std::atomic<bool> terminated_;
};

} // namespace utils