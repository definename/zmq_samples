#include "pch.h"
#include "Processor.h"

namespace utils
{

void CreateMsgList(types::ZmqMessageList& list, const std::vector<std::string>& dataList)
{
    for (const auto& data : dataList)
    {
        auto msg = std::make_shared<zmq::message_t>(data.size());
        memcpy(msg->data(), data.data(), data.size());
        list.push_back(msg);
    }
}

void SendMsgList(zmq::socket_t& socket, const types::ZmqMessageList& list)
{
    for (std::size_t i = 0; i != list.size() - 1; ++i)
    {
        if (!socket.send(*list.at(i), ZMQ_SNDMORE))
        {
            std::cerr << "Message cannot be sent at the moment" << std::endl;
            break;
        }
    }
    socket.send(*list.at(list.size() - 1));
}

bool ReceiveMsgList(
    zmq::socket_t& socket, types::ZmqMessageList& list)
{
    int more = 0;
    size_t moreSize = sizeof(more);
    do
    {
        auto msg = std::make_shared<zmq::message_t>();
        if (!socket.recv(msg.get(), ZMQ_NOBLOCK)) {
            return false;
        }

        list.push_back(msg);
        socket.getsockopt(ZMQ_RCVMORE, &more, &moreSize);
    } while (more);

    return true;
}

void BoundTo(zmq::socket_t& socket)
{
    std::string ep(256, '0');
    size_t length = ep.size();
    socket.getsockopt(ZMQ_LAST_ENDPOINT, &ep[0], &length);
    ep.erase(ep.begin() + length - 1, ep.end());
    std::cout << "Bound to endpoint: " << ep << std::endl;
    std::string port(ep.substr(ep.find_last_of(':') + 1));
    std::cout << "Bound to port: " << port << std::endl;
}

} // namespace utils