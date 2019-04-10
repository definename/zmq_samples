#include "pch.h"
#include "ZmqPublisher.h"

ZmqPublisher::ZmqPublisher(
    utils::types::ZmqContext& ctx, const std::string& addr, const std::string& port)
    : utils::PubSocket(ctx, addr, port)
{
    Run();
}

ZmqPublisher::~ZmqPublisher()
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

void ZmqPublisher::Process()
{
    while (isRunning_)
    {
        try
        {
            utils::types::ZmqMessageList msgList;
            if (msgQueue_.wait_dequeue_timed(msgList, std::chrono::milliseconds(100)))
            {
				logger::log->info("Message is being sent...");
                utils::SendMsgList(*pub_, msgList);
            }
        }
        catch (const std::exception& e)
        {
			logger::log->error("Publishing error: {}", e.what());
        }
        catch (...)
        {
			logger::log->error("Unhandled publishing error");
        }
    }
}

void ZmqPublisher::Publish(const utils::types::ZmqMessageList& msgList)
{
    msgQueue_.enqueue(msgList);
}

void ZmqPublisher::Stop()
{
    PubSocket::Stop();
}