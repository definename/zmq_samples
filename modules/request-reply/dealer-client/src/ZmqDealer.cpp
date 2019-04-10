#include "pch.h"
#include "ZmqDealer.h"

ZmqDealer::ZmqDealer(
    utils::types::ZmqContext& ctx,
    const std::string& addr,
    const std::string& port,
    const std::string& identity)
    : utils::DealerSocket(ctx, addr, port, identity)
{ }

ZmqDealer::~ZmqDealer()
{
    try
    {
        Stop();
    }
    catch (const std::exception& e)
    {
        logger::log->error("ZmqDealer destruction error: {}", e.what());
    }
}

void ZmqDealer::Run()
{
    DealerSocket::Run();

    receiver_ = std::thread([this]() -> void
    {
        while (IsRunning())
        {
            try
            {
                utils::types::ZmqMessageList msgList;
                if (!Receive(msgList))
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                    continue;
                }

                if (recvHandler_) {
                    recvHandler_(msgList);
                }
            }
            catch (const std::exception& e)
            {
				logger::log->error("Async receving error occurred: {}", e.what());
            }
        }
    });
}

void ZmqDealer::AsyncReceive(const ReceiveHandler& recvHandler)
{
    recvHandler_ = recvHandler;
}

void ZmqDealer::AsyncSend(const utils::types::ZmqMessageList& msgList)
{
    DealerSocket::Send(msgList);
}