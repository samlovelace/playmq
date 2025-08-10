#ifndef PLAYERSTATE_HPP
#define PLAYERSTATE_HPP

#include <nlohmann/json.hpp>

struct PlayerState
{
    PlayerState(int id, float x, float y) : id(id), x(x), y(y) {}

    int id; 
    float x; 
    float y;

    nlohmann::json toJson()
    {
        nlohmann::json json; 
        json["id"] = id; 
        json["x"] = x; 
        json["y"] = y; 

        return json; 
    }

    static PlayerState fromJson(nlohmann::json& aJson)
    {
        PlayerState state(aJson["id"], aJson["x"], aJson["y"]); 
        return state; 
    }
};

#endif