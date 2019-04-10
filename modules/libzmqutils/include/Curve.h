#pragma once

namespace utils
{

void CreateKeyPair(std::string& z85PublicKey, std::string& z85PrivateKey)
{
	z85PublicKey.resize(40, 0);
	z85PrivateKey.resize(40, 0);

	int rc = zmq_curve_keypair(&z85PublicKey[0], &z85PrivateKey[0]);
	if (rc != 0)
		throw zmq::error_t();
}

void SaveKeyPair(
	const std::string& z85PublicKey,
	const std::string& z85PrivateKey,
	const std::string& z85Path)
{
	std::string z85PublicPath(z85Path + "\\z85Public.dat");
	std::ofstream file;
	file.open(&z85PublicPath[0], std::ios::out | std::ios::binary);
	if (!file)
		throw std::runtime_error("Unable to open public key file: " + z85PublicPath);

	file << z85PublicKey;
	file.close();

	std::string z85PrivatePath(z85Path + "\\z85Private.dat");
	file.open(&z85PrivatePath[0], std::ios::out | std::ios::binary);
	if (!file)
		throw std::runtime_error("Unable to open private key file: " + z85PrivatePath);

	file << z85PrivateKey;
	file.close();
}

void LoadKeyPair(
	std::string& z85PublicKey,
	std::string& z85PrivateKey,
	const std::string& z85Path)
{
	std::ifstream file;
	file.open(&z85PublicKey[0], std::ios::in | std::ios::binary);
	if (!file)
		throw std::runtime_error("Unable to open public key file: " + z85PublicKey);

	file >> z85PublicKey;
	file.close();

	//! Loads secret key.
	file.open(&z85PrivateKey[0], std::ios::in | std::ios::binary);
	if (!file)
		throw std::runtime_error("Unable to open private key file: " + z85PrivateKey);

	file >> z85PrivateKey;
	file.close();
}

} // namespace utils
