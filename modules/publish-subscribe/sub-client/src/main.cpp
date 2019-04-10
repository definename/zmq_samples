#include "pch.h"
#include "ZmqSubscriber.h"

using ZmqSubscriberPtr = std::shared_ptr<ZmqSubscriber>;

int main(int argc, TCHAR* argv[])
{
    try
    {
		logger::InitializeLog("console");

        utils::types::ZmqContext ctx(3);
        ZmqSubscriberPtr sub = std::make_shared<ZmqSubscriber>(ctx, "tcp://127.0.0.1", "21001");
        sub->DoOnMessageList([](const utils::types::ZmqMessageList& msgList)
        {
			std::ostringstream ss;
            for (const auto& msg : msgList)
            {
                std::string data(reinterpret_cast<const char*>(msg->data()), msg->size());
				ss << data << " ";
            }
			logger::log->trace("{}", ss.str());
        });
        sub->WaitForTerminationRequest();

		logger::UninitializeLog();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Client error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}