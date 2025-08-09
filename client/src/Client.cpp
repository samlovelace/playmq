
#include "Client.h"
#include <iostream> 
#include <thread> 
#include <nlohmann/json.hpp>

Client::Client(const std::string& aServerIp) : mContext(1), mServerIp(aServerIp)
{
    mInputHandler = std::make_shared<InputHandler>(); 
    mRenderer = std::make_shared<Renderer>(mInputHandler); 
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

    std::string addr = "tcp://" + mServerIp + ":5555";
    mReqJoinSocket.connect(addr);

    nlohmann::json request; 
    //request["request"] = "available_games"; 
    request["request"] = "join"; 
    request["game"] = "tank"; 

    mReqJoinSocket.send(zmq::buffer(request.dump()), zmq::send_flags::none);

    zmq::message_t reply; 
    mReqJoinSocket.recv(reply, zmq::recv_flags::none); // blocks until recvs 

    nlohmann::json replyJson = nlohmann::json::parse(reply.to_string()); 

    if(replyJson["join"] == "success")
    {
        std::cout << "Joining the game...\n";

        // launch thread to hear game state
        mThreads.push_back(std::thread(&Client::gameStateRecvLoop, this)); 

        // launch thread to publish user input to server 
    }

    mThreads.push_back(std::thread(&Client::run, this)); 
}

void Client::run()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
}

void Client::gameStateRecvLoop()
{
    zmq::socket_t gameStateRecv = zmq::socket_t(mContext, zmq::socket_type::sub); 
    std::string addr = "tcp://" + mServerIp + ":5556";
    gameStateRecv.connect(addr); 
    gameStateRecv.set(zmq::sockopt::subscribe, "state");

    while(true)
    {
        zmq::message_t topic_msg;
        zmq::message_t data_msg;

        // Receive topic
        gameStateRecv.recv(topic_msg, zmq::recv_flags::none);
        std::string topic(static_cast<char*>(topic_msg.data()), topic_msg.size());

        // Receive message
        gameStateRecv.recv(data_msg, zmq::recv_flags::none);
        nlohmann::json replyJson = nlohmann::json::parse(data_msg.to_string());

        mPlayerSates.clear(); 
        
        for(auto& player : replyJson["players"])
        {
            mPlayerSates.push_back(PlayerState::fromJson(player)); 
        }
        
    }
}

