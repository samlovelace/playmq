#ifndef CLIENT_H
#define CLIENT_H
 
#include <zmq.hpp>
#include <vector> 
#include <thread> 
#include "Renderer.h"
 
class Client 
{ 
public:
    Client();
    ~Client();

    void launch(); 

    std::shared_ptr<Renderer> getRenderer(){return mRenderer; }

private:

    void run(); 

    zmq::context_t mContext; 
    zmq::socket_t mReqJoinSocket; 

    std::shared_ptr<Renderer> mRenderer; 

    std::vector<std::thread> mThreads; 


};
#endif //CLIENT_H