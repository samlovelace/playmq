
#include "Server.h"

int main(int argc, char* argv[])
{
    std::string serverIp = "127.0.0.1"; 

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--ip" && i + 1 < argc) {
            serverIp = argv[++i]; // grab next arg
        }
    }

    Server svr(serverIp); 
    svr.run(); 
}