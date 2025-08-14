
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
    float speed = 10; 

    mState.x += speed * mInput.moveX; 
    mState.y += speed * mInput.moveY; 
}