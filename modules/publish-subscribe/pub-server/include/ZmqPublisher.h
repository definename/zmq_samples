#pragma once

class ZmqPublisher
    : public utils::PubSocket
    , public utils::App
{
    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    ZmqPublisher(utils::types::ZmqContext& ctx, const std::string& addr, const std::string& port);
    //! Destructor.
    ~ZmqPublisher();

    //
    // Construction and destruction.
    //
public:
    //! Publishes message list.
    void Publish(const utils::types::ZmqMessageList& msgList);

    //
    // Private interface.
    //
private:
    //! Processes messages.
    void Process() override;
    //! Stops.
    void Stop() override;

    //
    // Private data members.
    //
private:
    //! Message queue.
    moodycamel::BlockingConcurrentQueue<utils::types::ZmqMessageList> msgQueue_;
};