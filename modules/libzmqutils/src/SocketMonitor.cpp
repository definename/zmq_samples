#include "pch.h"
#include "SocketMonitor.h"

namespace utils
{

SocketMonitor::SocketMonitor(zmq::socket_t& socket, const std::string& addr)
	: addr_(addr)
    , accepted_(0)
{
	if (addr_.empty()) {
		throw std::runtime_error("Invalid socket monitor address");
	}

	f_ = std::async(
		std::launch::async,
		std::bind(
			static_cast<void(SocketMonitor::*)(zmq::socket_t &, const std::string &, int)>(&SocketMonitor::monitor),
			this,
			std::ref(socket),
			addr_.c_str(),
			ZMQ_EVENT_ALL));
}

SocketMonitor::~SocketMonitor(void)
{
	try
	{
		abort();
		if (f_.valid()) {
			f_.wait();
		}
	}
	catch (const std::exception& e)
	{
		logger::log->error("Socket monitor destruction error:  {}", e.what());
	}
}

void SocketMonitor::on_monitor_started()
{
	logger::log->info("Socket monitor has been started on address: {}", addr_);
}

void SocketMonitor::on_event_connected(const zmq_event_t &event, const char* addr)
{
	logger::log->info("The socket: {} has successfully connected to a remote peer", addr);
}

void SocketMonitor::on_event_connect_delayed(const zmq_event_t &event, const char* addr)
{
	logger::log->info("A connect request on the socket: {} is pending", addr);
}

void SocketMonitor::on_event_connect_retried(const zmq_event_t &event, const char* addr)
{
	logger::log->info("Socket: {} connection request failed, and is now being retried", addr);
}

void SocketMonitor::on_event_listening(const zmq_event_t &event, const char* addr)
{
	logger::log->info("The socket: {} was successfully bound to a network interface", addr);
}

void SocketMonitor::on_event_bind_failed(const zmq_event_t &event, const char* addr)
{
	logger::log->info("The socket: {} could not bind to a given interface", addr);
}

void SocketMonitor::on_event_accepted(const zmq_event_t &event, const char* addr)
{
	logger::log->info("The socket: {} has accepted a connection from a remote peer", addr);
    ++accepted_;
}

void SocketMonitor::on_event_accept_failed(const zmq_event_t &event, const char* addr)
{
	logger::log->info("The socket: {} has rejected a connection from a remote peer", addr);
}

void SocketMonitor::on_event_closed(const zmq_event_t &event, const char* addr)
{
	logger::log->info("The socket: {} was closed", addr);
}

void SocketMonitor::on_event_close_failed(const zmq_event_t &event, const char* addr)
{
	logger::log->info("The socket: {} close failed", addr);
}

void SocketMonitor::on_event_disconnected(const zmq_event_t &event, const char* addr)
{
	logger::log->info("The socket: {} was disconnected unexpectedly", addr);
    --accepted_;
}

void SocketMonitor::on_event_unknown(const zmq_event_t &event, const char* addr)
{
	logger::log->info("Unknown socket: {} event", addr);
}

} // namespace utils
