#include "pch.h"
#include "ZmqRequest.h"

ZmqRequest::ZmqRequest(
    utils::types::ZmqContext& ctx, const std::string& addr, const std::string& port)
    : utils::ReqSocket(ctx, addr, port)
{
    Run();
}

ZmqRequest::~ZmqRequest()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
		logger::log->error("ZmqRequest destruction error: {}", e.what());
    }
}

void ZmqRequest::Process()
{
    while (IsRunning())
    {
        try
        {
            utils::types::ZmqMessageList outMsgList;
            if (!msgQueue_.wait_dequeue_timed(outMsgList, std::chrono::milliseconds(100)))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

			logger::log->info("Message is being sent...");
            utils::SendMsgList(*req_, outMsgList);

            while (IsRunning())
            {
                utils::types::ZmqMessageList inMsgList;
                if (!utils::ReceiveMsgList(*req_, inMsgList))
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    continue;
                }

                respHandler_(inMsgList);
                break;
            }
        }
        catch (const std::exception& e)
        {
			logger::log->error("ZmqRequest error: {}", e.what());
        }
        catch (...)
        {
			logger::log->error("Unhandled ZmqRequest error");
        }
    }
}

void ZmqRequest::DoOnResponse(const RespHandler& respHandler)
{
    respHandler_ = respHandler;
}

void ZmqRequest::Send(const utils::types::ZmqMessageList& msgList)
{
    msgQueue_.enqueue(msgList);
}