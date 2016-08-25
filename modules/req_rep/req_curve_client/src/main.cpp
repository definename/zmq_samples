#include "pch.h"
#include "ZmqMonitor.h"

//////////////////////////////////////////////////////////////////////////
// Represents ZMQ_REQ-ZMQ_REP pattern.
//

int _tmain(int argc, TCHAR* argv[])
{
	try
	{
		boost::filesystem::path root(argv[0]);
		root.remove_filename();
		root /= "server_public_key.txt";

		//! Loads server public key.
		std::fstream file(root.c_str());
		if (file.fail())
			throw std::runtime_error("Unable to open file");

		std::string z85_server_public_key;
		file >> z85_server_public_key;
		file.close();

		//! Generates client public/secret keys.
		std::string z85_public_key(40, 0);
		std::string z85_secret_key(40, 0);

		int rc = zmq_curve_keypair(&z85_public_key[0], &z85_secret_key[0]);
		if (rc != 0)
			std::cout << zmq_strerror(zmq_errno()) << std::endl;

		//! Initializes client.
		zmq::context_t context(1);
		zmq::socket_t socket(context, ZMQ_REQ);

		socket.setsockopt(ZMQ_CURVE_SERVERKEY, z85_server_public_key.data(), z85_server_public_key.size());
		socket.setsockopt(ZMQ_CURVE_PUBLICKEY, z85_public_key.data(), z85_public_key.size());
		socket.setsockopt(ZMQ_CURVE_SECRETKEY, z85_secret_key.data(), z85_secret_key.size());


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

		const char* addr = "tcp://127.0.0.1:6666";
		socket.connect(addr);

		std::cout << "Client connects to address: " << addr << "\n" << std::endl;
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
	}
	catch (const std::exception& e)
	{
		std::cerr << "CLIENT error: " << e.what() << std::endl;
	}


	return 0;
}