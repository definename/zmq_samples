#include "pch.h"
#include "ZmqMonitor.h"

//////////////////////////////////////////////////////////////////////////
// Represents ZMQ_REQ-ZMQ_REP pattern.
//

int _tmain(int argc, TCHAR* argv[])
{
	try
	{
		zmq::context_t context(1);
		zmq::socket_t socket (context, ZMQ_REQ);

		ZmqMonitor monitor;
		boost::shared_future<void> f;
		f = boost::async(boost::launch::async,
			boost::bind(
			static_cast<void (ZmqMonitor::*) (zmq::socket_t &socket, const char *addr_, int events)> (&ZmqMonitor::monitor),
			&monitor,
			boost::ref(socket),
			"inproc://c_monitor.rep",
			ZMQ_EVENT_ALL));

		boost::thread t([&f] () -> void { f.wait(); });

		const char* addr = "tcp://127.0.0.1:21038";
		socket.connect(addr);

		std::cout << "CLIENT has been initialized" << std::endl;
		while (true)
		{
			// Send request.
			std::string str("HELLO");
			zmq::message_t request (str.size());
			memcpy ((void*)request.data(), str.c_str(), str.size());

			socket.send(request);

			// Receive and print.
			char buffer[256];
			size_t size = socket.recv (buffer, 256, 0);
			if (size == -1)
				continue;
			if (size > 255)
				size = 255;
			buffer[size] = 0;

			std::cout << "Received: " << buffer << std::endl;
		}

		//! Stops monitor.
		monitor.abort();
		f.get();
	}
	catch (const std::exception& e)
	{
		std::cerr << "CLIENT error: " << e.what() << std::endl;
	}


	return 0;
}