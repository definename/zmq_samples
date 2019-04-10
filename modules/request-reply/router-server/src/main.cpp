#include "pch.h"
#include "ZmqRouter.h"

using ZmqReplyPtr = std::shared_ptr<ZmqRouter>;

int main()
{
    try
    {
		logger::InitializeLog("console");

        /*
        auto reqHandler = [](const utils::types::ZmqMessageList& inMsgList) -> utils::types::ZmqMessageList
        {
            for (const auto& msg : inMsgList)
            {
                std::string data(reinterpret_cast<const char*>(msg->data()), msg->size());
                LOG->info("{} ", data);
            }

            utils::types::ZmqMessageList outMsgList;
            utils::CreateMsgList(outMsgList, { "Welcome", "to", "ZMQ" });

            return outMsgList;
        };

        utils::types::ZmqContext ctx(3);
        ZmqReplyPtr zmqReply = std::make_shared<ZmqReply>(ctx, "tcp://*", "21002", reqHandler);
        zmqReply->WaitForTerminationRequest();
        */

		logger::UninitializeLog();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}