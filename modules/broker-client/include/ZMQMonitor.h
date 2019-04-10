#pragma once

class ZMQMonitor : public zmq::monitor_t
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	ZMQMonitor();
	//! Destructor.
	~ZMQMonitor();

	//
	// Private interface.
	//
private:
	//! Handles monitor start.
	void on_monitor_started();
	//! Handles connection to the remove peer.
	void on_event_connected(const zmq_event_t &event_, const char* addr_);
	//! Handles pending event.
	void on_event_connect_delayed(const zmq_event_t &event_, const char* addr_);
	//! Handles reconnect.
	void on_event_connect_retried(const zmq_event_t &event_, const char* addr_);
	//! Handles bind to a network interface.
	void on_event_listening(const zmq_event_t &event_, const char* addr_);
	//! Handles binding error to a network interface.
	void on_event_bind_failed(const zmq_event_t &event_, const char* addr_);
	//! Handles connection from a remote peer
	void on_event_accepted(const zmq_event_t &event_, const char* addr_);
	//! Handles rejection a connection from a remote peer.
	void on_event_accept_failed(const zmq_event_t &event_, const char* addr_);
	//! Handles close event.
	void on_event_closed(const zmq_event_t &event_, const char* addr_);
	//! Handles close fail event.
	void on_event_close_failed(const zmq_event_t &event_, const char* addr_);
	//! Handles unexpected disconnection.
	void on_event_disconnected(const zmq_event_t &event_, const char* addr_);
	//! Handles unknown event.
	void on_event_unknown(const zmq_event_t &event_, const char* addr_);
};