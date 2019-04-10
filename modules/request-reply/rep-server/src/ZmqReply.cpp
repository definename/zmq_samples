#include "pch.h"
#include "ZmqReply.h"

ZmqReply::ZmqReply(
    utils::types::ZmqContext& ctx,
    const std::string& addr,
    const std::string& port,
    ReqHandler reqHandler)
    : utils::RepSocket(ctx, addr, port)
    , reqHandler_(reqHandler)
{
    Run();
}

ZmqReply::~ZmqReply()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
        logger::log->error("ZmqReply destruction error: {}", e.what());
    }
}

void ZmqReply::Process()
{
    while (IsRunning())
    {
        try
        {
            utils::types::ZmqMessageList inMsgList;
            if (!utils::ReceiveMsgList(*rep_, inMsgList))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            auto outMsgList = reqHandler_(inMsgList);
            utils::SendMsgList(*rep_, outMsgList);
        }
        catch (const std::exception& e)
        {
			logger::log->error("ZmqReply error: {}", e.what());
        }
        catch (...)
        {
			logger::log->error("Unhandled ZmqReply error");
        }
    }
}