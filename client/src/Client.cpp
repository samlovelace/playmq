
#include "Client.h"
#include <iostream> 
#include <thread> 
#include <nlohmann/json.hpp>

Client::Client() : mContext(1), mRenderer(std::make_shared<Renderer>())
{

}

Client::~Client()
{
    for(auto& t : mThreads)
    {
        if(t.joinable())
        {
            t.join(); 
        }
    }
}

void Client::launch()
{
    // connect to server and request available games
    mReqJoinSocket = zmq::socket_t(mContext, zmq::socket_type::req);

    std::string addr = "tcp://127.0.0.1:5555";
    mReqJoinSocket.connect(addr);

    nlohmann::json request; 
    request["request"] = "available_games"; 

    mReqJoinSocket.send(zmq::buffer(request.dump()), zmq::send_flags::none);

    zmq::message_t reply; 
    mReqJoinSocket.recv(reply, zmq::recv_flags::none); // blocks until recvs 

    std::cout << "Response from server: " << reply.to_string() << std::endl; 

    mThreads.push_back(std::thread(&Client::run, this)); 
}

void Client::run()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
}

