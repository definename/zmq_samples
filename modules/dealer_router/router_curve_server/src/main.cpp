#include "pch.h"
#include "ZMQMonitor.h"

//////////////////////////////////////////////////////////////////////////
// Represents ZMQ_REQ-ZMQ_REP pattern.
//

typedef std::pair<std::string, std::string> KeyPair;

const std::string publicKeyName = "server_public_key.txt";
const std::string secretKeyName = "server_secret_key.txt";

void GenerateCurveKeyPair(
	boost::filesystem::path publicKeyPath,
	boost::filesystem::path secretKeyPath);

KeyPair LoadCurveKeyPair(
	boost::filesystem::path publicKeyPath,
	boost::filesystem::path secretKeyPath);

int _tmain(int argc, TCHAR* argv[])
{
	try
	{
		boost::filesystem::path root(argv[0]);
		root.remove_filename();

		boost::filesystem::path publicKeyPath(root);
		publicKeyPath /= publicKeyName;
		boost::filesystem::path secretKeyPath(root);
		secretKeyPath /= secretKeyName;

		if (!boost::filesystem::exists(publicKeyPath) || !boost::filesystem::exists(secretKeyPath))
			GenerateCurveKeyPair(publicKeyPath, secretKeyPath);

		KeyPair keyPair = LoadCurveKeyPair(publicKeyPath, secretKeyPath);

		zmq::context_t context(1);
		zmq::socket_t socket(context, ZMQ_ROUTER);

		int curve(1);
		socket.setsockopt(ZMQ_CURVE_SERVER, &curve, sizeof(curve));
		socket.setsockopt(ZMQ_CURVE_SECRETKEY, keyPair.second.data(), keyPair.second.size());
		std::string idOwn("1b7f3602-0a63-4c73-9dde-7875b0f91bfd");
		socket.setsockopt(ZMQ_IDENTITY, idOwn.data(), idOwn.size());

		ZMQMonitor monitor;
		boost::shared_future<void> f;
		f = boost::async(boost::launch::async,
			boost::bind(
			static_cast<void (ZMQMonitor::*) (zmq::socket_t&, const char*, int)> (&ZMQMonitor::monitor),
			&monitor,
			boost::ref(socket),
			"inproc://s_monitor.rep",
			ZMQ_EVENT_ALL));

		boost::thread t([&f] () -> void { f.wait(); });

		const char* addr = "tcp://*:6666";
		//const char* addr = "tcp://*:21031";
		socket.bind(addr);

		std::cout << "Server listening on address: " << addr << "\n" << std::endl;
		while (true)
		{
			int more = 0;
			std::size_t size = sizeof(more);
			do
			{
				zmq::message_t request;
				if (socket.recv(&request, ZMQ_DONTWAIT))
				{
					std::string m;
					m.resize(request.size());
					memcpy(&m[0], request.data(), m.size());
					std::cout << m << std::endl;

					socket.getsockopt(ZMQ_RCVMORE, &more, &size);
				}
			}
			while (more);

			zmq_sleep(1);

			//! Send reply back to client
			//! Id must be same on client side (set it with ZMQ_IDENTITY)
			std::string id("e342fcfd-064f-4ec4-8e56-3de3f6418995");
			zmq::message_t idMessage(id.size());
			memcpy(reinterpret_cast<void*>(idMessage.data()), id.c_str(), id.size());
			socket.send(idMessage, ZMQ_SNDMORE);

			std::string str("WELCOME");
			zmq::message_t reply(str.size());
			memcpy(reinterpret_cast<void*>(reply.data()), str.c_str(), str.size());
			socket.send(reply);
		}

		f.get();
	}
	catch (const std::exception& e)
	{
		std::cerr << "SERVER error: " << e.what() << std::endl;
	}


	return 0;
}

void GenerateCurveKeyPair(
	boost::filesystem::path publicKeyPath,
	boost::filesystem::path secretKeyPath)
{
	//! Generates public/secret keys.
	std::string z85_public_key(40, 0);
	std::string z85_secret_key(40, 0);

	int rc = zmq_curve_keypair(&z85_public_key[0], &z85_secret_key[0]);
	if (rc != 0)
		std::cout << zmq_strerror(zmq_errno()) << std::endl;

	std::ofstream file;
	//! Saves server public key into file.
	file.open(publicKeyPath.c_str(), std::ios::out);
	if (file.fail())
		throw std::runtime_error("Unable to open public key file");

	file << z85_public_key;
	file.close();

	//! Saves server secret key into file.
	file.open(secretKeyPath.c_str(), std::ios::out);
	if (file.fail())
		throw std::runtime_error("Unable to open secret key file");

	file << z85_secret_key;
	file.close();
}

KeyPair LoadCurveKeyPair(
	boost::filesystem::path publicKeyPath,
	boost::filesystem::path secretKeyPath)
{
	std::fstream file;
	//! Loads public key.
	file.open(publicKeyPath.c_str());
	if (file.fail())
		throw std::runtime_error("Unable to open public key file");

	std::string z85_public_key;
	file >> z85_public_key;
	file.close();

	//! Loads secret key.
	file.open(secretKeyPath.c_str());
	if (file.fail())
		throw std::runtime_error("Unable to open secret key file");

	std::string z85_secret_key;
	file >> z85_secret_key;
	file.close();

	return std::make_pair(z85_public_key, z85_secret_key);
}