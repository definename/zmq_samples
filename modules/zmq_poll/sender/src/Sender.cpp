#include "pch.h"
#include "Sender.h"

Sender::Sender()
	: context_(1)
	, socket_(context_, ZMQ_DEALER)
{
	std::string connectUrl = "tcp://127.0.0.1:8001";
	try
	{
		socket_.connect(connectUrl.c_str());
	}
	catch (const zmq::error_t& e)
	{
		std::cerr << "Socket connection error: " << e.what() << std::endl;
	}

	std::cout << "Sender initialized" << std::endl;
}

Sender::~Sender()
{
	socket_.close();
}

void Sender::SendMessage()
{
	try
	{
		std::string messageStr("Sender message");
		zmq::message_t message(messageStr.size());
		memcpy(message.data(), messageStr.data(), message.size());
		int rc = socket_.send(message);
		std::cout << "Send return code: " << rc << std::endl;
	}
	catch (const zmq::error_t& e)
	{
		std::cout << "Error during send message: " << e.what() << std::endl;
	}
}