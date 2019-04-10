#include "pch.h"
#include "SubSocket.h"
#include "SocketMonitor.h"

namespace utils
{

SubSocket::SubSocket(types::ZmqContext& ctx, const std::string& addr, const std::string& port)
    : isRunning_(false)
    , sub_(std::make_shared<zmq::socket_t>(ctx, ZMQ_SUB))
    , monitor_(std::make_shared<SocketMonitor>(*sub_, "inproc://monitor-sub"))
{
    Connect(addr, port);
}

SubSocket::~SubSocket()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
		logger::log->info("Sub socket destruction error: {}", e.what());
    }
}

void SubSocket::Connect(const std::string& addr, const std::string& port)
{
    if (!sub_)
        throw std::runtime_error("Invalid sub socket pointer");

    std::string ep;
    ep += addr;
    ep += ":";
    ep += port;

    sub_->connect(ep.c_str());
}

void SubSocket::Run()
{
    isRunning_ = true;
    worker_ = std::thread(&SubSocket::Process, this);
}

void SubSocket::Stop()
{
    if (isRunning_)
    {
        isRunning_ = false;
        if (worker_.joinable())
            worker_.join();
    }
}

bool SubSocket::IsRunning() const
{
    return isRunning_;
}

} // namespace utils
