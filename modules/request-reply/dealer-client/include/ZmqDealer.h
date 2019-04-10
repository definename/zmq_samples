#pragma once

class ZmqDealer
    : public utils::DealerSocket
    , public utils::App
{
    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    ZmqDealer(
        utils::types::ZmqContext& ctx,
        const std::string& addr,
        const std::string& port,
        const std::string& identity);
    //! Destructor.
    ~ZmqDealer();

    //
    // Public types.
    //
public:
    //! Receive handler.
    using ReceiveHandler = std::function<void(utils::types::ZmqMessageList)>;

    //
    // Public interface.
    //
public:
    //! Receives messages asynchronously.
    void AsyncReceive(const ReceiveHandler& recvHandler);
    //! Sends message asynchronously.
    void AsyncSend(const utils::types::ZmqMessageList& msgList);

    //
    // Private interface.
    //
protected:
    //! Starts.
    void Run() override;

    //
    // Private data members.
    //
private:
    //! Receive handler.
    ReceiveHandler recvHandler_;
    //! Receiving thread.
    utils::types::ZmqWorker receiver_;
};