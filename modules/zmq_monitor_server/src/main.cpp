#include "pch.h"
#include "ZmqMonitor.h"

//////////////////////////////////////////////////////////////////////////
// Represents ZMQ_REQ-ZMQ_REP pattern.
//

int main()
{
	try
	{
		zmq::context_t context(1);
		zmq::socket_t socket(context, ZMQ_REP);

		std::shared_ptr<ZmqMonitor> monitor(new ZmqMonitor());
		boost::shared_future<void> f;
		f = boost::async(boost::launch::async,
			boost::bind(
			static_cast<void (ZmqMonitor::*) (zmq::socket_t&, const char*, int)> (&ZmqMonitor::monitor),
			&(*monitor),
			boost::ref(socket),
			"inproc://s_monitor.rep",
			ZMQ_EVENT_ALL));

		boost::thread t([&f] () -> void { f.wait(); });

		const char* addr = "tcp://*:21038";
		socket.bind(addr);

		std::cout << "SERVER has been initialized" << std::endl;
		while (true)
		{
			zmq::message_t request;

			//  Wait for next request from client
			char buffer[256];
			size_t size = socket.recv (buffer, 256, 0);
			if (size == -1)
				continue;
			if (size > 255)
				size = 255;
			buffer[size] = 0;

			std::cout << "Received: "<< buffer << std::endl;
			zmq_sleep(1);

			// Send reply back to client
			std::string str("WELCOME");
			zmq::message_t reply(str.size());
			memcpy ((void*)reply.data(), str.c_str(), str.size());

			socket.send (reply);
		}

		//! Stops monitor.
		monitor->abort();
		f.get();
	}
	catch (const std::exception& e)
	{
		std::cerr << "SERVER error: " << e.what() << std::endl;
	}


	return 0;
}