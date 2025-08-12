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
    Server(const std::string& anIP);
    ~Server();

    void run(); 

private:

    std::string mIP; 

    zmq::context_t mContext; 

    std::vector<std::thread> mThreads; 
    std::vector<std::string> mGames; 

    zmq::socket_t mClientResponseSocket; 
    RateController mRequestPollRate; 
    void clientRequestHandleLoop();
    bool handleRequest(const zmq::message_t& aRequest, nlohmann::json& aResponse);

    std::map<int, std::unique_ptr<Player>> mPlayers; 


    std::atomic<bool> mIsRunning; 
    bool isRunning() {return mIsRunning.load(); }

    void tankGameLoop(); 
    void broadcastGameLoop(); 
    void clientInputRecvLoop(); 
   
};
#endif //SERVER_H