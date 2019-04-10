#include "pch.h"
#include "ZmqPublisher.h"


using ZmqPublisherPtr = std::shared_ptr<ZmqPublisher>;

int main()
{
    try
    {
		logger::InitializeLog("console");

        utils::types::ZmqContext ctx(3);
        ZmqPublisherPtr pub = std::make_shared<ZmqPublisher>(ctx, "tcp://*", "21001");

		auto worker = std::thread([&pub]()
		{
			for (int i = 0; i != 5; ++i)
			{
				utils::types::ZmqMessageList msgList;
				utils::CreateMsgList(msgList, { "Hello", "Every", "One" });

				std::this_thread::sleep_for(std::chrono::seconds(1));
				pub->Publish(msgList);
			}
			pub->Terminate();
		});

        pub->WaitForTerminationRequest();
		worker.join();

		logger::UninitializeLog();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}