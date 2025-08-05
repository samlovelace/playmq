
#include "Server.h"
#include <iostream>

Server::Server() : mContext(1), mIsRunning(true), mRequestPollRate(10), mGames({"Snake", "Tanks", "Test"})
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
    mThreads.push_back(std::thread(&Server::clientRequestHandleLoop, this)); 

    while(isRunning())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
}

void Server::clientRequestHandleLoop()
{
    mClientResponseSocket = zmq::socket_t(mContext, zmq::socket_type::rep); 
    // TODO: make configurable IP/Port and Poll Rate
    
    std::string addr = "tcp://127.0.0.1:5555";
    mClientResponseSocket.bind(addr); 
    std::cout << "Listening for connections on: " << addr << std::endl; 

    while(isRunning())
    {
        mRequestPollRate.start(); 

        zmq::message_t request; 
        mClientResponseSocket.recv(request); 

        nlohmann::json response; 
        if(!handleRequest(request, response))
        {
            std::cout << "Failed to handle request for: " << request.to_string() << std::endl; 
            continue; 
        } 

        mClientResponseSocket.send(zmq::buffer(response.dump(2)), zmq::send_flags::none);

        mRequestPollRate.block(); 
    }

}

bool Server::handleRequest(const zmq::message_t& aRequest, nlohmann::json& aResponse)
{
    nlohmann::json reqJson = nlohmann::json::parse(aRequest.to_string()); 

    if("available_games" == reqJson["request"])
    {
        aResponse["response"] = reqJson["request"];
        auto games = nlohmann::json::array(); 

        for(const auto& gameName : mGames)
        {
            games.push_back(gameName); 
        }

        aResponse["games"] = games; 
    }
    else
    {
        std::cout << "request not supported" << std::endl; 
        return false; 
    }

    return true; 
}