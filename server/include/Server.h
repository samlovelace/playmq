#ifndef SERVER_H
#define SERVER_H
 
#include <vector> 
#include <thread> 
#include <atomic> 
#include "RateController.hpp"
#include "Player.h"

#include <nlohmann/json.hpp>
#include <zmq.hpp>

class Server 
{ 
public:
    Server();
    ~Server();

    void run(); 

private:

    zmq::context_t mContext; 

    std::vector<std::thread> mThreads; 
    std::vector<std::string> mGames; 

    zmq::socket_t mClientResponseSocket; 
    RateController mRequestPollRate; 
    void clientRequestHandleLoop();
    bool handleRequest(const zmq::message_t& aRequest, nlohmann::json& aResponse);

    std::vector<std::unique_ptr<Player>> mPlayers; 


    std::atomic<bool> mIsRunning; 
    bool isRunning() {return mIsRunning.load(); }
   
};
#endif //SERVER_H