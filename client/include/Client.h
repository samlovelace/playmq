#ifndef CLIENT_H
#define CLIENT_H
 
#include <zmq.hpp>
 
class Client 
{ 
public:
    Client();
    ~Client();

    void run(); 

private:

    zmq::context_t mContext; 
    zmq::socket_t mReqJoinSocket; 


};
#endif //CLIENT_H