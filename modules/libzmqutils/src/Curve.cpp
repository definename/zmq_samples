#include "pch.h"
#include "Curve.h"

namespace utils
{

//! Generate a new CURVE key pair.
void CreateKeyPair(std::string& z85PublicKey, std::string& z85PrivateKey);
//! Saves CURVE key pair to the file.
void SaveKeyPair(
	const std::string& z85PublicKey,
	const std::string& z85PrivateKey,
	const std::string& z85Path);
//! Load CURVE key pair from file.
void LoadKeyPair();

} // namespace utils
