#ifndef SERVER_H
#define SERVER_H
 
#include <vector> 
#include <thread> 
#include <atomic> 
#include "RateController.hpp"

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


    std::atomic<bool> mIsRunning; 
    bool isRunning() {return mIsRunning.load(); }
   
};
#endif //SERVER_H