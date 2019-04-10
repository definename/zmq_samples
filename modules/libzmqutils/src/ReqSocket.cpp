#include "pch.h"
#include "ReqSocket.h"
#include "SocketMonitor.h"

namespace utils
{

ReqSocket::ReqSocket(utils::types::ZmqContext& ctx, const std::string& addr, const std::string& port)
    : isRunning_(false)
    , req_(std::make_shared<zmq::socket_t>(ctx, ZMQ_REQ))
    , monitor_(std::make_shared<SocketMonitor>(*req_, "inproc://monitor-req"))
{
    Connect(addr, port);
}

ReqSocket::~ReqSocket()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
		logger::log->error("Request socket destruction error: {}", e.what());
    }
}

void ReqSocket::Connect(const std::string& addr, const std::string& port)
{
    if (!req_) {
        throw std::runtime_error("Invalid request socket pointer");
    }

    std::string ep;
    ep += addr;
    ep += ":";
    ep += port;

    req_->connect(ep.c_str());
}

void ReqSocket::Run()
{
    isRunning_ = true;
    worker_ = std::thread(&ReqSocket::Process, this);
}

void ReqSocket::Stop()
{
    if (isRunning_)
    {
        isRunning_ = false;
        if (worker_.joinable()) {
            worker_.join();
        }
    }
}

bool ReqSocket::IsRunning() const
{
    return isRunning_;
}

} // namespace utils
