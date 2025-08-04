
#include "Client.h"
#include <iostream> 

Client::Client() : mContext(1)
{

}

Client::~Client()
{

}

void Client::run()
{
    mReqJoinSocket = zmq::socket_t(mContext, zmq::socket_type::req);

    std::string addr = "tcp://127.0.0.1:5555";
    mReqJoinSocket.connect(addr);

    // request to join sent to the server 
    mReqJoinSocket.send(zmq::buffer("join_request"), zmq::send_flags::none);

    zmq::message_t reply; 
    mReqJoinSocket.recv(reply, zmq::recv_flags::none); // blocks until recvs 

    std::cout << "Response from server: " << reply.to_string() << std::endl; 

}

