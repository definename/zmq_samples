#pragma once

class Sender
{
	//! Construction and destruction.
public:
	//! Constructor.
	Sender();
	//! Destructor.
	~Sender();

	//! Public interface.
public:
	//! Sends message.
	void SendMessage();


	//! Private data members.
private:
	//! Zmq context.
	zmq::context_t context_;
	//! Zmq socket.
	zmq::socket_t socket_;
};