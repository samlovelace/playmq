#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerState.hpp"
#include "InputFrame.hpp"
 
class Player 
{ 
public:
    Player(int anId);
    ~Player();

    bool update(); 
    PlayerState getState() {return mState; }
    void setInput(const InputFrame& anInput) {mInput = anInput; }

private:
    int mId; 

    PlayerState mState; 
    InputFrame mInput; 
   
};
#endif //PLAYER_H