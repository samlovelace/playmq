
#include "Client.h"
#include <iostream> 
#include <thread> 
#include <nlohmann/json.hpp>
#include "RateController.hpp"

Client::Client(const std::string& aServerIp) : mContext(1), mServerIp(aServerIp), mId(-1), mIsRunning(true)
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
    std::cout << "Connecting to server on " << addr << "\n"; 
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

        mId = replyJson["id"]; 

        // launch thread to hear game state
        mThreads.push_back(std::thread(&Client::gameStateRecvLoop, this)); 

        // launch thread to publish user input to server 
        mThreads.push_back(std::thread(&Client::sendUserInputLoop, this)); 
    }

    //mThreads.push_back(std::thread(&Client::run, this)); 
}

void Client::kill()
{
    std::cout << "Killing client....\n"; 
    mIsRunning = false; 

    nlohmann::json request; 
    request["request"] = "disconnect"; 
    request["id"] = mId; 
    mReqJoinSocket.send(zmq::buffer(request.dump()), zmq::send_flags::none); 

    zmq::message_t reply; 
    mReqJoinSocket.recv(reply, zmq::recv_flags::none); // blocks until recvs 

    nlohmann::json replyJson = nlohmann::json::parse(reply.to_string());

    std::cout << "Server says: " << replyJson["message"] << std::endl; 
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

    while(isRunning())
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

        mRenderer->setLatestGameState(mPlayerSates);        
    }
}

// TODO: find a way to avoid always sending all input frames. Feel like I
//       only need to send when it changes? 
void Client::sendUserInputLoop()
{
    zmq::socket_t inputSendSocket = zmq::socket_t(mContext, zmq::socket_type::push); 
    std::string addr = "tcp://" + mServerIp + ":5557"; 
    inputSendSocket.connect(addr); 

    RateController* inputSendRate = new RateController(10); 
    
    while(isRunning())
    {
        inputSendRate->start(); 

        // get latest user input from renderer
        InputFrame latest = mRenderer->getLatestUserInput();

        if(-1 == latest.clientId)
        {
            latest.clientId = mId; 
        }

        nlohmann::json frame = latest.toJson(); 

        // convert to zmq buffer and send 
        inputSendSocket.send(zmq::buffer(frame.dump()), zmq::send_flags::none);
        
        inputSendRate->block(); 
    }

    delete inputSendRate; 
}
