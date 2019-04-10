#pragma once

namespace utils
{

// Forward declaration.
class SocketMonitor;

namespace types
{

//! Socket monitor pointer type.
using ZmqMonitorPtr = std::shared_ptr<SocketMonitor>;
//! Zmq message pointer type.
using ZmqMessagePtr = std::shared_ptr<zmq::message_t>;
//! Zmq message list type.
using ZmqMessageList = std::vector<ZmqMessagePtr>;

//! Zmq socket pointer type.
using ZmqSocketPtr = std::shared_ptr<zmq::socket_t>;
//! Zmq context pointer type.
using ZmqContext = zmq::context_t;
//! Zmq worker type.
using ZmqWorker = std::thread;

} // namespace types
} // namespace utils
