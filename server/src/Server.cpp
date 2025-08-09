
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
    mThreads.push_back(std::thread(&Server::tankGameLoop, this)); 
    mThreads.push_back(std::thread(&Server::broadcastGameLoop, this)); 

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
    else if ("join" == reqJson["request"])
    {
        if("tank" == reqJson["game"])
        {
            //TODO: add player to game
            int id = mPlayers.size() + 1; 
            auto player = std::make_unique<Player>(id); 
            mPlayers.push_back(std::move(player)); 

            std::cout << "Added player " << id << "\n"; 

            aResponse["join"] = "success"; 
            aResponse["id"] = id; 
        }
    }
    else
    {
        std::cout << "request not supported" << std::endl; 
        return false; 
    }

    return true; 
}

void Server::tankGameLoop()
{
    auto gameRate = new RateController(5);

    while(isRunning())
    {
        gameRate->start(); 

        for(auto& player : mPlayers)
        {
            player->update(); 
        }

        gameRate->block(); 
    }
}

void Server::broadcastGameLoop()
{
    RateController* broadcastRate = new RateController(5); 

    auto gameStatePublisher = zmq::socket_t(mContext, zmq::socket_type::pub); 
    std::string addr = "tcp://127.0.0.1:5556";
    gameStatePublisher.bind(addr);

    std::string topic = "state"; 

    PlayerState playerState(0, 0, 0); 
    nlohmann::json players; 
    auto stateToPublish = nlohmann::json::array();

    while(isRunning())
    {
        broadcastRate->start(); 

        if(!mPlayers.empty())
        {
            for(auto& player : mPlayers)
            {
                playerState = player->getState(); 

                stateToPublish.push_back(playerState.toJson());
            }

            players["players"] = stateToPublish; 

            gameStatePublisher.send(zmq::buffer(topic), zmq::send_flags::sndmore); 
            gameStatePublisher.send(zmq::buffer(players.dump(2)), zmq::send_flags::dontwait);
        }

        broadcastRate->block(); 
    }
}