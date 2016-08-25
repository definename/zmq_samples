#include "pch.h"
#include "ZmqMonitor.h"

ZmqMonitor::ZmqMonitor()
{ }

ZmqMonitor::~ZmqMonitor()
{ }

void ZmqMonitor::on_monitor_started()
{
	std::cout << "on_monitor_started" << std::endl;
}

void ZmqMonitor::on_event_connected(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_connected" << std::endl;
}

void ZmqMonitor::on_event_connect_delayed(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_connect_delayed" << std::endl;
}

void ZmqMonitor::on_event_connect_retried(const zmq_event_t &event_, const char* addr_)
{ 
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_connect_retried" << std::endl;
}
void ZmqMonitor::on_event_listening(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_listening" << std::endl;
}
void ZmqMonitor::on_event_bind_failed(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_bind_failed" << std::endl;
}
void ZmqMonitor::on_event_accepted(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_accepted" << std::endl;
}
void ZmqMonitor::on_event_accept_failed(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_accept_failed" << std::endl;
}
void ZmqMonitor::on_event_closed(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_closed" << std::endl;
}
void ZmqMonitor::on_event_close_failed(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_close_failed" << std::endl;
}
void ZmqMonitor::on_event_disconnected(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_disconnected" << std::endl;
}
void ZmqMonitor::on_event_unknown(const zmq_event_t &event_, const char* addr_)
{
	std::cout << "addr: " << addr_ << std::endl;
	std::cout << "on_event_unknown" << std::endl;
}