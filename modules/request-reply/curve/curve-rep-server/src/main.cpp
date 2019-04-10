#include "pch.h"

typedef std::pair<std::string, std::string> KeyPair;

const std::string publicKeyName = "server_public_key.txt";
const std::string secretKeyName = "server_secret_key.txt";

void GenerateCurveKeyPair(
	boost::filesystem::path publicKeyPath,
	boost::filesystem::path secretKeyPath);

KeyPair LoadCurveKeyPair(
	boost::filesystem::path publicKeyPath,
	boost::filesystem::path secretKeyPath);

int main(int argc, char* argv[])
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
		zmq::socket_t socket(context, ZMQ_REP);

		int curve(1);
		socket.setsockopt(ZMQ_CURVE_SERVER, &curve, sizeof(curve));
		socket.setsockopt(ZMQ_CURVE_SECRETKEY, keyPair.second.data(), keyPair.second.size());

		utils::types::ZmqMonitorPtr monitor = std::make_shared<
			utils::SocketMonitor>(socket, "inproc://monitor-curve-reply");

		const char* addr = "tcp://*:6666";
		socket.bind(addr);

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
	}
	catch (const std::exception& e)
	{
		std::cerr << "Server error occurred: " << e.what() << std::endl;
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