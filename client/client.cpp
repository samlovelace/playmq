
#include "Client.h"

int main(int argc, char* argv[])
{
    std::string serverIp = "127.0.0.1"; 

    for(int i = 1; i < argc; i++)
    {
        if("--server" == argv[i] && i + 1 < argc)
        {
            serverIp = argv[i++]; 
        }
    }

    Client client(serverIp); 
    client.launch(); 
    client.getRenderer()->run(); 
}