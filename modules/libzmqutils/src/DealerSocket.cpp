#include "pch.h"
#include "DealerSocket.h"
#include "SocketMonitor.h"
#include "Processor.h"

namespace utils
{

DealerSocket::DealerSocket(
    utils::types::ZmqContext& ctx,
    const std::string& addr,
    const std::string& port,
    const std::string& identity)
    : isRunning_(false)
    , dealer_(std::make_shared<zmq::socket_t>(ctx, ZMQ_DEALER))
    , monitor_(std::make_shared<SocketMonitor>(*dealer_, "inproc://monitor-dealer"))
{
    dealer_->setsockopt(ZMQ_IDENTITY, identity.data(), identity.size());
    Connect(addr, port);
}

DealerSocket::~DealerSocket()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
        logger::log->error("DealerSocket socket destruction error: {}", e.what());
    }
}

void DealerSocket::Connect(const std::string& addr, const std::string& port)
{
    if (!dealer_) {
        throw std::runtime_error("Invalid DealerSocket socket pointer");
    }

    std::string ep;
    ep += addr;
    ep += ":";
    ep += port;

    dealer_->connect(ep.c_str());
}

void DealerSocket::Run()
{
    isRunning_ = true;
    recvThread_ = std::thread(&DealerSocket::DoReceive, this);
    sendThread_ = std::thread(&DealerSocket::DoSend, this);
}

void DealerSocket::Stop()
{
    if (isRunning_)
    {
        isRunning_ = false;
        recvThread_.join();
        sendThread_.join();
    }
}

bool DealerSocket::IsRunning() const
{
    return isRunning_;
}

void DealerSocket::DoReceive()
{
    while (IsRunning())
    {
        try
        {
            utils::types::ZmqMessageList msgList;
            if (!utils::ReceiveMsgList(*dealer_, msgList))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            // virtual Receive();
            recvQueue_.enqueue(msgList);
        }
        catch (const std::exception& e)
        {
			logger::log->error("ZmqDealer do receving error {}", e.what());
        }
    }
}

void DealerSocket::DoSend()
{
    while (IsRunning())
    {
        try
        {
            utils::types::ZmqMessageList msgList;
            if (!sendQueue_.wait_dequeue_timed(msgList, std::chrono::milliseconds(100)))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            utils::SendMsgList(*dealer_, msgList);
        }
        catch (const std::exception& e)
        {
			logger::log->error("ZmqDealer sending error {}", e.what());
        }
    }
}

bool DealerSocket::Receive(utils::types::ZmqMessageList& msgList)
{
    return recvQueue_.try_dequeue(msgList);
}

void DealerSocket::Send(const utils::types::ZmqMessageList& msgList)
{
    recvQueue_.enqueue(msgList);
}

} // namespace utils
