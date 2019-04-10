#pragma once

class ZmqRequest
    : public utils::ReqSocket
    , public utils::App
{
    //
    // Public types.
    //
public:
    //! Response handler type.
    using RespHandler = std::function<void(const utils::types::ZmqMessageList&)>;

    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    ZmqRequest(
        utils::types::ZmqContext& ctx, const std::string& addr, const std::string& port);
    //! Destructor.
    ~ZmqRequest();

    //
    // Public interface.
    //
public:
    //! Response handler.
    void DoOnResponse(const RespHandler& respHandler);
    //! Sends request.
    void Send(const utils::types::ZmqMessageList& msgList);

    //
    // Private interface.
    //
private:
    //! Processes messages.
    void Process() override;

    //
    // Private data members.
    //
private:
    //! Response handler.
    RespHandler respHandler_;
    //! Message queue.
    moodycamel::BlockingConcurrentQueue<utils::types::ZmqMessageList> msgQueue_;
};