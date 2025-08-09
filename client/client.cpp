
#include "Client.h"

int main(int argc, char* argv[])
{
    std::string serverIp = "127.0.0.1"; 

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--server" && i + 1 < argc) {
            serverIp = argv[++i]; // grab next arg
        }
    }

    Client client(serverIp); 
    client.launch(); 
    client.getRenderer()->run(); 
}