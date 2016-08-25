#include "pch.h"
#include "Sender.h"

int main()
{
	Sender sender;

	while (true)
	{
		sender.SendMessage();
		zmq_sleep(4);
	}

	return 0;
}