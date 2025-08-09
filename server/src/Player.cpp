
#include "Player.h"
#include <iostream> 

Player::Player(int anId) : mId(anId), mState(mId, 0, 0)
{

}

Player::~Player()
{

}

bool Player::update()
{   
    mState.x += 1; 
    std::cout << "Updating player " << mId << std::endl; 
}