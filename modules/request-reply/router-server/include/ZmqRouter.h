#pragma once

class ZmqRouter
    : public utils::RouterSocket
    , public utils::App
{
    //
    // Construction and destruction.
    //
public:
    //! Constructor.
    ZmqRouter(
        utils::types::ZmqContext& ctx,
        const std::string& addr,
        const std::string& port,
        const std::string& identity);
    //! Destructor.
    ~ZmqRouter();

    //
    // Private interface.
    //
private:
    //! Processes messages.
    void Process() override;
};