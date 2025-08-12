#ifndef INPUTFRAME_HPP
#define INPUTFRAME_HPP

#include <nlohmann/json.hpp>

struct InputFrame 
{
    InputFrame() : clientId(-1), moveX(0), moveY(0) {}

    //uint32_t seq;
    int clientId;
    //uint64_t timestampMs;

    int moveX;            // -1..1 (WASD/left stick mapped here)
    int moveY;            // -1..1

    nlohmann::json toJson()
    {
        nlohmann::json frame; 
        frame["clientId"] = clientId; 

        frame["moveX"] = moveX; 
        frame["moveY"] = moveY;  

        return frame; 
    }

};

#endif 