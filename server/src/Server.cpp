
#include "Server.h"
#include <iostream>
#include <nlohmann/json.hpp>

Server::Server() : mContext(1), mIsRunning(true), mJoinPollRate(10)
{

}

Server::~Server()
{
    for(auto& t : mThreads)
    {
        if(t.joinable())
        {
            t.join(); 
        }
    }
}

void Server::run()
{
    mThreads.push_back(std::thread(&Server::clientConnectionHandle, this)); 

    while(isRunning())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
}

void Server::clientConnectionHandle()
{
    mJoinSocket = zmq::socket_t(mContext, zmq::socket_type::rep); 
    // TODO: make configurable IP/Port and Poll Rate
    
    std::string addr = "tcp://127.0.0.1:5555";
    mJoinSocket.bind(addr); 
    std::cout << "Listening for connections on: " << addr << std::endl; 

    while(isRunning())
    {
        mJoinPollRate.start(); 

        zmq::message_t request; 
        mJoinSocket.recv(request); 

        std::cout << "Recvd: " << request.to_string() << std::endl; 

        int playerId = mPlayers.size() + 1; 
        auto player = std::make_unique<Player>(playerId); 
        mPlayers.push_back(std::move(player));
        
        nlohmann::json response;
        response["action"] = "accepted"; 
        response["playerId"] = playerId; 

        mJoinSocket.send(zmq::buffer(response.dump(2)), zmq::send_flags::none);
        
        std::cout << "Accepted Player " << playerId << "\n"; 

        mJoinPollRate.block(); 
    }

}