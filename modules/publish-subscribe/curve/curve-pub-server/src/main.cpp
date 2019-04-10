#include "pch.h"
#include "ZmqMonitor.h"

//////////////////////////////////////////////////////////////////////////
// Represents ZMQ_SUB-ZMQ_PUB pattern.
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

		//! Initializes server.
		zmq::context_t context(1);
		zmq::socket_t socket(context, ZMQ_PUB);

		int curve(1);
		socket.setsockopt(ZMQ_CURVE_SERVER, &curve, sizeof(curve));
		socket.setsockopt(ZMQ_CURVE_SECRETKEY, keyPair.second.data(), keyPair.second.size());

		ZmqMonitor monitor;
		boost::async(boost::launch::async,
			boost::bind(
			static_cast<void (ZmqMonitor::*) (zmq::socket_t &socket, const char *addr_, int events)> (&ZmqMonitor::monitor),
			&monitor,
			boost::ref(socket),
			"inproc://server_monitor.rep",
			ZMQ_EVENT_ALL));

		const char* addr = "tcp://127.0.0.1:6666";
		socket.bind(addr);

		std::cout << "CURVE SERVER successfully initialized" << std::endl;

		while (true)
		{
			std::string str("curve message");
			zmq::message_t mes(str.size());
			memcpy ((void *) mes.data (), str.c_str(), str.size());
			zmq_sleep(1);
			socket.send(mes);
			std::cout << "Buffer has been sent" << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
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