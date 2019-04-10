#include "pch.h"
#include "ZmqSubscriber.h"

ZmqSubscriber::ZmqSubscriber(
    utils::types::ZmqContext& ctx, const std::string& addr, const std::string& port)
    : utils::SubSocket(ctx, addr, port)
{
    const int timeout = 5000;
    sub_->setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    // Subscribe to all incoming data.
    std::string prefix;
   sub_->setsockopt(ZMQ_SUBSCRIBE, prefix.data(), prefix.size());
    // Subscribes to all messages beginning with the specified prefix.
//     std::string prefix("Hello");
//     sub_->setsockopt(ZMQ_SUBSCRIBE, prefix.data(), prefix.size());

   Run();
}

ZmqSubscriber::~ZmqSubscriber()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
		logger::log->error("Subscriber destruction error: {}", e.what());
    }
}

void ZmqSubscriber::Process()
{
    while (isRunning_)
    {
        utils::types::ZmqMessageList msgList;
        if (utils::ReceiveMsgList(*sub_, msgList)) {
            msgHandler_(msgList);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ZmqSubscriber::Stop()
{
    SubSocket::Stop();
}

void ZmqSubscriber::DoOnMessageList(const MessageHandler& msgHandler)
{
    msgHandler_ = msgHandler;
}