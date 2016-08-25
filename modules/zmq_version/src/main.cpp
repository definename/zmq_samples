#include "pch.h"

int _tmain(int argc, TCHAR* argv)
{
	int major, minor, patch;
	zmq::version(&major, &minor, &patch);
	std::cout << "ZMQ version: " << major << "."<< minor << "." << patch << std::endl;
	return 0;
}