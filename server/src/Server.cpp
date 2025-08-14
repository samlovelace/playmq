
#include "Server.h"
#include <iostream>
#include "InputFrame.hpp"

Server::Server(const std::string& anIP) : mContext(1), mIP(anIP), mIsRunning(true), mRequestPollRate(10), mGames({"Snake", "Tanks", "Test"})
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
    mThreads.push_back(std::thread(&Server::clientInputRecvLoop, this));  

    while(isRunning())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
}

void Server::clientRequestHandleLoop()
{
    mClientResponseSocket = zmq::socket_t(mContext, zmq::socket_type::rep); 
    // TODO: make configurable IP/Port and Poll Rate
    
    std::string addr = "tcp://" +  mIP + ":5555";
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
            int id = mPlayers.size() + 1; 
            auto player = std::make_unique<Player>(id); 
            mPlayers.insert({id, std::move(player)}); 

            std::cout << "Added player " << id << "\n"; 

            aResponse["join"] = "success"; 
            aResponse["id"] = id; 
        }
    }
    else if ("disconnect" == reqJson["request"])
    {
        int clientId = reqJson["id"]; 

        if(mPlayers.find(clientId) != mPlayers.end())
        {
            // client exists, remove
            mPlayers.erase(clientId); 
        }

        aResponse["disconnect"] = "success"; 
        aResponse["message"] = "rest in peace homie";   
        
        std::cout << "Removed Player " << clientId << "\n"; 
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
    auto gameRate = new RateController(30);

    while(isRunning())
    {
        gameRate->start(); 

        for(auto& [id, player] : mPlayers)
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
    std::string addr = "tcp://" +  mIP + ":5556";
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
            for(auto& [id, player] : mPlayers)
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

void Server::clientInputRecvLoop()
{
    zmq::socket_t clientInputRecvSocket = zmq::socket_t(mContext, zmq::socket_type::pull); 
    std::string addr = "tcp://" + mIP + ":5557"; 
    clientInputRecvSocket.bind(addr); 

    RateController* clientInputRecvRate = new RateController(10); 

    while(isRunning())
    {
        clientInputRecvRate->start(); 

        zmq::message_t input; 
        clientInputRecvSocket.recv(input, zmq::recv_flags::none);

        nlohmann::json inputJson = nlohmann::json::parse(input.to_string()); 

        if(!setPlayersLatestInput(InputFrame::fromJson(inputJson)))
            std::cout << "Failed to update input for unknown player id\n"; 

        clientInputRecvRate->block(); 
    }

}

bool Server::setPlayersLatestInput(const InputFrame& anInput)
{
    if(mPlayers.empty())
    {
        // kinda hacky
        return true; 
    }

    if(mPlayers.find(anInput.clientId) == mPlayers.end())
    {   
        std::cout << "Unknown player id: " << anInput.clientId << "\n"; 
        return false; 
    }

    mPlayers.at(anInput.clientId)->setInput(anInput); 
    return true; 
}