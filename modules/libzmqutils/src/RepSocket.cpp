#include "pch.h"
#include "RepSocket.h"
#include "SocketMonitor.h"

namespace utils
{

RepSocket::RepSocket(types::ZmqContext& ctx, const std::string& addr, const std::string& port)
    : isRunning_(false)
    , rep_(std::make_shared<zmq::socket_t>(ctx, ZMQ_REP))
    , monitor_(std::make_shared<SocketMonitor>(*rep_, "inproc://monitor-rep"))
{
    Bind(addr, port);
}

RepSocket::~RepSocket()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
		logger::log->error("Reply socket destruction error: {}", e.what());
    }
}

void RepSocket::Bind(const std::string& addr, const std::string& port)
{
    if (!rep_) {
        throw std::runtime_error("Invalid reply socket pointer");
    }

    std::string ep;
    ep += addr;
    ep += ":";
    ep += port;

    rep_->bind(ep.c_str());
}

void RepSocket::Run()
{
    isRunning_ = true;
    worker_ = std::thread(&RepSocket::Process, this);
}

void RepSocket::Stop()
{
    if (isRunning_)
    {
        isRunning_ = false;
        if (worker_.joinable()) {
            worker_.join();
        }
    }
}

bool RepSocket::IsRunning() const
{
    return isRunning_;
}

} // namespace utils
