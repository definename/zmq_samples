#include "pch.h"
#include "ZMQMonitor.h"
#include "MessageBroker.pb.h"

//! Sends ping.
void Ping(zmq::socket_t& socket);
//! Packs protobuf message.
void Pack(const quicktech::broker::Protocol& protocol, std::string& packet);
//! Unpacks protobuf message.
void Unpack(quicktech::broker::Protocol& protocol, const zmq::message_t& message);

int _tmain(int argc, TCHAR* argv[])
{
	try
	{
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		zmq::context_t context(1);
		zmq::socket_t socket(context, ZMQ_DEALER);
		std::string id(quicktech::broker::Service().default_instance().id());
		socket.setsockopt(ZMQ_IDENTITY, id.data(), id.size());

		ZMQMonitor monitor;
		boost::shared_future<int> f;
		f = boost::async(boost::launch::async, [&monitor, &socket]() -> int
		{
			monitor.monitor(socket, "inproc://c_monitor.rep", ZMQ_EVENT_ALL);
			return EXIT_SUCCESS;
		});

		socket.connect("tcp://192.168.88.253:21036");
		//Ping(socket);

		int count = 5;
		while (count != 0)
		{
			std::queue<zmq::message_t> list;
			uint64_t more = 0;
			std::size_t size = sizeof(more);
			do
			{
				list.push(zmq::message_t());
				socket.recv(&list.back()/*, ZMQ_DONTWAIT*/);
				socket.getsockopt(ZMQ_RCVMORE, &more, &size);
			}
			while (more);

			quicktech::broker::Protocol protocol;
			Unpack(protocol, list.back());
			if (!protocol.has_type() || !protocol.has_msg())
				throw std::runtime_error("Invalid request");

			quicktech::broker::Protocol_Type type = protocol.type();
			switch (type)
			{
			case quicktech::broker::Protocol_Type_NetScan:
			{
				std::cout << "Net scan protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_RemoteInstaller:
			{
				std::cout << "Remote installer protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_AgentLogin:
			{
				std::cout << "Agent login protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_UninstallDevice:
			{
				std::cout << "Uninstall device protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_RemoteConnection:
			{
				std::cout << "Remote connection protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_NetworkDeviceDescription:
			{
				std::cout << "Network device description protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_TestCredentials:
			{
				std::cout << "Test credentials protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_Ticket:
			{
				std::cout << "Ticket protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_NewUser:
			{
				std::cout << "New user protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_Script:
			{
				std::cout << "Script protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_Activity:
			{
				std::cout << "Activity protocol type" << std::endl;
				break;
			}
			case quicktech::broker::Protocol_Type_Heartbeat:
			{
				zmq::message_t message(protocol.msg().size());
				memcpy(message.data(), protocol.msg().data(), protocol.msg().size());

				quicktech::broker::Heartbeat heartbeat;
				if (!heartbeat.ParseFromArray(message.data(), static_cast<int>(message.size())))
					throw std::runtime_error("Failed to parse protocol buffer");
				if (!heartbeat.has_pong() || !heartbeat.has_sequenceno() || !heartbeat.has_timestamp())
					throw std::runtime_error("Invalid heartbeat response");

				std::cout << "sequenceno: " << heartbeat.sequenceno() << std::endl;
				std::cout << "timestamp: " << heartbeat.timestamp() << std::endl;

				std::cout << "Pong..." << std::endl;

				--count;
				zmq_sleep(1);

				Ping(socket);

				break;
			}
			case quicktech::broker::Protocol_Type_UserRequest:
			{
				std::cout << "User list request" << std::endl;
				break;
			}
			default:
			{
				std::cerr << "Unknown protocol type" << std::endl;
				break;
			}
			}
		}

		::google::protobuf::ShutdownProtobufLibrary();
		monitor.abort();
		return f.get();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error occurred: " << e.what() << std::endl;
	}

	return EXIT_FAILURE;
}

void Ping(zmq::socket_t& socket)
{
	quicktech::broker::Heartbeat heartbeat;
	heartbeat.set_ping(true);
	heartbeat.set_sequenceno(11111111);

	zmq::message_t ping(heartbeat.ByteSize());
	if (!heartbeat.SerializeToArray(ping.data(), static_cast<int>(ping.size())))
		throw std::runtime_error("Failed to serialize with protocol buffer");

	quicktech::broker::Protocol protocol;
	protocol.set_type(quicktech::broker::Protocol_Type_Heartbeat);
	protocol.set_msg(ping.data(), ping.size());

	std::string packed;
	Pack(protocol, packed);
	zmq::message_t message(packed.size());
	memcpy(message.data(), packed.data(), packed.size());
	socket.send(message);

	std::cout << "Ping..." << std::endl;
}

void Pack(const quicktech::broker::Protocol& protocol, std::string& packet)
{
	::google::protobuf::io::StringOutputStream sstream(&packet);
	::google::protobuf::io::GzipOutputStream::Options opt;
	opt.format = ::google::protobuf::io::GzipOutputStream::ZLIB;
	opt.compression_level = 6;
	::google::protobuf::io::GzipOutputStream gstream(&sstream, opt);

	if (!protocol.SerializeToZeroCopyStream(&gstream))
		throw std::runtime_error("Failed to serialize with protocol buffer");

	gstream.Flush();
}

void Unpack(quicktech::broker::Protocol& protocol, const zmq::message_t& message)
{
	::google::protobuf::io::ArrayInputStream astream(
		message.data(), static_cast<int>(message.size()));
	::google::protobuf::io::GzipInputStream gstream(&astream);

	if (!protocol.ParseFromZeroCopyStream(&gstream))
		throw std::runtime_error("Failed to parse with protocol buffer");
}