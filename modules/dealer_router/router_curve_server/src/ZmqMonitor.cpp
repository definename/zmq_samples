#include "pch.h"
#include "ZmqMonitor.h"

ZMQMonitor::ZMQMonitor()
{ }

ZMQMonitor::~ZMQMonitor()
{ }

void ZMQMonitor::on_monitor_started()
{
	std::cout << "Socket monitor has been started" << std::endl;
}

void ZMQMonitor::on_event_connected(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "The socket: " << addr_ << " has successfully connected to a remote peer." << std::endl;
}

void ZMQMonitor::on_event_connect_delayed(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "A connect request on the socket: " << addr_ << " is pending" << std::endl;
}

void ZMQMonitor::on_event_connect_retried(const zmq_event_t &event_, const char* addr_)
{ 
	std::cout << "Socket: " << addr_ << " connection request failed, and is now being retried" << std::endl;
}

void ZMQMonitor::on_event_listening(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "The socket: " << addr_ << " was successfully bound to a network interface" << std::endl;
}

void ZMQMonitor::on_event_bind_failed(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "The socket: " << addr_ << " could not bind to a given interface" << std::endl;
}

void ZMQMonitor::on_event_accepted(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "The socket: " << addr_ << " has accepted a connection from a remote peer" << std::endl;
}

void ZMQMonitor::on_event_accept_failed(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "The socket: " << addr_ << " has rejected a connection from a remote peer" << std::endl;
}

void ZMQMonitor::on_event_closed(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "The socket: " << addr_ << " was closed" << std::endl;
}

void ZMQMonitor::on_event_close_failed(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "The socket: " << addr_ << " close failed" << std::endl;
}

void ZMQMonitor::on_event_disconnected(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "The socket: " << addr_ << " was disconnected unexpectedly" << std::endl;
}

void ZMQMonitor::on_event_unknown(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "Unknown socket: " << addr_ << " event" << std::endl;
}