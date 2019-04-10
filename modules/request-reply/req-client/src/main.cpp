#include "pch.h"
#include "ZmqRequest.h"

using ZmqRequestPtr = std::shared_ptr<ZmqRequest>;

int main(int argc, TCHAR* argv[])
{
    try
    {
		logger::InitializeLog("console");

        utils::types::ZmqContext ctx(3);
        ZmqRequestPtr zmqRequest = std::make_shared<ZmqRequest>(ctx, "tcp://127.0.0.1", "21002");

        auto req = [zmqRequest]() -> void
        {
            utils::types::ZmqMessageList msgList;
            utils::CreateMsgList(msgList, { "Hello", "ZMQ" });
            zmqRequest->Send(msgList);
        };

        zmqRequest->DoOnResponse([req](const utils::types::ZmqMessageList& msgList) -> void
        {
            std::string resp;
            for (const auto& msg : msgList)
            {
                std::string data(reinterpret_cast<const char*>(msg->data()), msg->size());
                resp += data;
                resp += " ";
            }
            logger::log->info("{}", resp);

            req();
        });

        req();
        zmqRequest->WaitForTerminationRequest();

		logger::UninitializeLog();
    }
    catch (const std::exception& e)
    {
        std::cerr << "ZmqRequest error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}