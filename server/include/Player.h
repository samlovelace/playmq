#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerState.hpp"
 
class Player 
{ 
public:
    Player(int anId);
    ~Player();

    bool update(); 
    PlayerState getState() {return mState; }

private:
    int mId; 

    PlayerState mState; 
   
};
#endif //PLAYER_H