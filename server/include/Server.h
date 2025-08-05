#ifndef SERVER_H
#define SERVER_H
 
#include <vector> 
#include <thread> 
#include <atomic> 
#include "RateController.hpp"
#include "Player.h"

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

    zmq::socket_t mJoinSocket; 
    RateController mJoinPollRate; 
    void clientConnectionHandle();

    std::vector<std::unique_ptr<Player>> mPlayers; 


    std::atomic<bool> mIsRunning; 
    bool isRunning() {return mIsRunning.load(); }
   
};
#endif //SERVER_H