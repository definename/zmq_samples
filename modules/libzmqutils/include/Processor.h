#pragma once

#include "Types.h"

namespace utils
{

//! Constructs zmq message list from given data list.
void CreateMsgList(types::ZmqMessageList& list, const std::vector<std::string>& dataList);
//! Receives message list on given socket.
bool ReceiveMsgList(zmq::socket_t& socket, types::ZmqMessageList& list);
//! Writes message list to given socket.
void SendMsgList(zmq::socket_t& socket, const types::ZmqMessageList& list);
//! Discover which port we were actually bound to.
void BoundTo(zmq::socket_t& socket);

} // namespace utils
