#include "pch.h"
#include "ZmqRouter.h"

ZmqRouter::ZmqRouter(
    utils::types::ZmqContext& ctx,
    const std::string& addr,
    const std::string& port,
    const std::string& identity)
    : utils::RouterSocket(ctx, addr, port, identity)
{
    Run();
}

ZmqRouter::~ZmqRouter()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
        logger::log->error("ZmqRouter destruction error: {}", e.what());
    }
}

void ZmqRouter::Process()
{
    while (IsRunning())
    {
        try
        {
            /*
            utils::types::ZmqMessageList inMsgList;
            if (!utils::ReceiveMsgList(*rep_, inMsgList))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            auto outMsgList = reqHandler_(inMsgList);
            utils::SendMsgList(*rep_, outMsgList);
            */
        }
        catch (const std::exception& e)
        {
			logger::log->error("ZmqRouter error: {}", e.what());
        }
        catch (...)
        {
			logger::log->error("Unhandled ZmqReply error");
        }
    }
}