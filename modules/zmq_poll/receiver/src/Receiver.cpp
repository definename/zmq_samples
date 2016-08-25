#include "pch.h"
#include "Receiver.h"

Receiver::Receiver()
	: context_(10)
	, dataSocket_(context_, ZMQ_DEALER)
{
	std::string bindUrl = "tcp://*:8001";
	pollSocket[0].socket = dataSocket_;
	pollSocket[0].events = ZMQ_POLLIN;

	try
	{
		dataSocket_.bind(bindUrl.c_str());
	}
	catch (const zmq::error_t& e)
	{
		std::cerr << "Socket connection error: " << e.what() << std::endl;
	}

	std::cout << "Dealer initialized" << std::endl;
}

Receiver::~Receiver()
{
	dataSocket_.close();
}

void Receiver::DealerLoop()
{
	try
	{
		while (true)
		{
			try
			{
				int rc = zmq::poll(pollSocket, 1, 100);
				if (rc == 0)
					std::cout << "No events have been signaled" << std::endl;
				else
					std::cout << "Poll result: " << rc << std::endl;
			}
			catch (const zmq::error_t& e)
			{
				std::cerr << "Zmq poll request error: " << e.what();
			}

			if (pollSocket[0].revents & ZMQ_POLLIN)
			{
				std::cout << "ZMQ POLLIN event signaled" << std::endl;

				// Receive and print.
				char buffer[256];
				size_t size = dataSocket_.recv(buffer, 256, 0);
				if (size == -1)
					continue;
				if (size > 255)
					size = 255;
				buffer[size] = 0;

				std::cout << "\tReceived:  "<< buffer << std::endl;
			}

			if (pollSocket[0].revents & ZMQ_POLLOUT)
			{
				std::cout << "ZMQ POLLOUT event signaled" << std::endl;
			}

			std::cout << "revent value: " << pollSocket[0].revents << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Dealer loop error: " << e.what() << std::endl;
	}
}