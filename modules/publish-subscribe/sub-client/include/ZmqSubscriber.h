#pragma once

class ZmqSubscriber 
    : public utils::SubSocket
    , public utils::App
{
    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    ZmqSubscriber(utils::types::ZmqContext& ctx, const std::string& addr, const std::string& port);
    //! Destructor.
    ~ZmqSubscriber();

    //
    // Public types.
    //
public:
    //! Message handler type.
    using MessageHandler = std::function<void(utils::types::ZmqMessageList)>;

    //
    // Public interface.
    //
public:
    //! Do on message list.
    void DoOnMessageList(const MessageHandler& msgHandler);

    //
    // Private interface.
    //
private:
    //! Processes messages.
    void Process() override;
    //! Stops.
    void Stop() override;

    //
    // Private types.
    //
private:
    //! Message handler.
    MessageHandler msgHandler_;
};