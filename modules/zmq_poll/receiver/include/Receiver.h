#pragma once

class Receiver
{
	//! Construction and destruction.
public:
	//! Constructor.
	Receiver();
	//! Destructor.
	~Receiver();

	//! Public interface.
public:
	//! Dealer's loop.
	void DealerLoop();

	//! Private types.
private:
	enum
	{
		PollItem = 1
	};

	//! Private data members.
private:
	//! Zmq context.
	zmq::context_t context_;
	//! Zmq socket.
	zmq::socket_t dataSocket_;
	//! Zmq poll socket.
	zmq::pollitem_t pollSocket[PollItem];
};