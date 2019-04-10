#include "pch.h"
#include "RouterSocket.h"
#include "SocketMonitor.h"

namespace utils
{

RouterSocket::RouterSocket(
    utils::types::ZmqContext& ctx,
    const std::string& addr,
    const std::string& port,
    const std::string& identity)
    : isRunning_(false)
    , router_(std::make_shared<zmq::socket_t>(ctx, ZMQ_ROUTER))
    , monitor_(std::make_shared<SocketMonitor>(*router_, "inproc://monitor-router"))
{
    router_->setsockopt(ZMQ_IDENTITY, identity.data(), identity.size());
    Connect(addr, port);
}

RouterSocket::~RouterSocket()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
		logger::log->error("RouterSocket socket destruction error: {}", e.what());
    }
}

void RouterSocket::Connect(const std::string& addr, const std::string& port)
{
    if (!router_) {
        throw std::runtime_error("Invalid RouterSocket socket pointer");
    }

    std::string ep;
    ep += addr;
    ep += ":";
    ep += port;

    router_->connect(ep.c_str());
}

void RouterSocket::Run()
{
    isRunning_ = true;
    worker_ = std::thread(&RouterSocket::Process, this);
}

void RouterSocket::Stop()
{
    if (isRunning_)
    {
        isRunning_ = false;
        if (worker_.joinable()) {
            worker_.join();
        }
    }
}

bool RouterSocket::IsRunning() const
{
    return isRunning_;
}

} // namespace utils
