#pragma once

class ZmqReply
    : public utils::RepSocket
    , public utils::App
{
    //
    // Public types.
    //
public:
    //! Request handler type.
    using ReqHandler = std::function<utils::types::ZmqMessageList(const utils::types::ZmqMessageList&)>;

    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    ZmqReply(
        utils::types::ZmqContext& ctx,
        const std::string& addr,
        const std::string& port,
        ReqHandler reqHandler);
    //! Destructor.
    ~ZmqReply();

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
    //! Request handler.
    ReqHandler reqHandler_;
};