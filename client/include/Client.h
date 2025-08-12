#ifndef CLIENT_H
#define CLIENT_H
 
#include <zmq.hpp>
#include <vector> 
#include <thread> 
#include "Renderer.h"
#include "InputHandler.h"
#include "PlayerState.hpp"


class Client 
{ 
public:
    Client(const std::string& aSeverIp);
    ~Client();

    void launch(); 
    void kill(); 

    std::shared_ptr<Renderer> getRenderer(){return mRenderer; }

private:

    void run(); 
    void gameStateRecvLoop(); 
    void sendUserInputLoop();

    bool isRunning() {return mIsRunning; }

    zmq::context_t mContext; 
    zmq::socket_t mReqJoinSocket; 

    std::string mServerIp; 
    int mId; 

    bool mIsRunning; 

    std::shared_ptr<Renderer> mRenderer; 
    std::shared_ptr<InputHandler> mInputHandler; 

    std::vector<std::thread> mThreads; 

    std::vector<PlayerState> mPlayerSates; 


};
#endif //CLIENT_H