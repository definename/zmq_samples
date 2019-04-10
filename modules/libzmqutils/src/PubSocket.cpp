#include "pch.h"
#include "PubSocket.h"
#include "SocketMonitor.h"

namespace utils
{

PubSocket::PubSocket(utils::types::ZmqContext& ctx, const std::string& addr, const std::string& port)
    : isRunning_(false)
    , pub_(std::make_shared<zmq::socket_t>(ctx, ZMQ_PUB))
    , monitor_(std::make_shared<SocketMonitor>(*pub_, "inproc://monitor-pub"))
{
    Bind(addr, port);
}

PubSocket::~PubSocket()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
		logger::log->info("Pub socket destruction error: {}", e.what());
    }
}

void PubSocket::Bind(const std::string& addr, const std::string& port)
{
    if (!pub_)
        throw std::runtime_error("Invalid pub socket pointer");

    std::string ep;
    ep += addr;
    ep += ":";
    ep += port;

    pub_->bind(ep.c_str());
}

void PubSocket::Run()
{
    isRunning_ = true;
    worker_ = std::thread(&PubSocket::Process, this);
}

void PubSocket::Stop()
{
    if (isRunning_)
    {
        isRunning_ = false;
		if (worker_.joinable()) {
			worker_.join();
		}
    }
}

bool PubSocket::IsRunning() const
{
    return isRunning_;
}

} // namespace utils
