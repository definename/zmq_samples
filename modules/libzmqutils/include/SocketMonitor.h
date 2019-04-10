#pragma once

namespace utils
{

class SocketMonitor : public zmq::monitor_t
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	SocketMonitor(zmq::socket_t& socket, const std::string& addr);
	//! Destructor.
	~SocketMonitor(void);

    //
    // Public interface.
    //
public:
    //! Returns acceptance flag.
    bool IsAccepted() {
        return accepted_ != 0;
    }

	//
	// Private interface.
	//
protected:
	//! Handles monitor start.
	void on_monitor_started();
	//! Handles connection to the remove peer.
	void on_event_connected(const zmq_event_t &event, const char* addr);
	//! Handles pending event.
	void on_event_connect_delayed(const zmq_event_t &event, const char* addr);
	//! Handles reconnect.
	void on_event_connect_retried(const zmq_event_t &event, const char* addr);
	//! Handles bind to a network interface.
	void on_event_listening(const zmq_event_t &event, const char* addr);
	//! Handles binding error to a network interface.
	void on_event_bind_failed(const zmq_event_t &event, const char* addr);
	//! Handles connection from a remote peer
	void on_event_accepted(const zmq_event_t &event, const char* addr);
	//! Handles rejection a connection from a remote peer.
	void on_event_accept_failed(const zmq_event_t &event, const char* addr);
	//! Handles close event.
	void on_event_closed(const zmq_event_t &event, const char* addr);
	//! Handles close fail event.
	void on_event_close_failed(const zmq_event_t &event, const char* addr);
	//! Handles unexpected disconnection.
	void on_event_disconnected(const zmq_event_t &event, const char* addr);
	//! Handles unknown event.
	void on_event_unknown(const zmq_event_t &event, const char* addr);

	//
	// Private data members.
	//
protected:
	//! Socket monitor future result.
	std::shared_future<void> f_;
	//! Socket monitor address.
	const std::string addr_;
    //! Acceptance counter.
    std::atomic<int> accepted_;
};

} // namespace utils
