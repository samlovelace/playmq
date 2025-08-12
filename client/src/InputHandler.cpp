
#include "InputHandler.h"
#include <iostream>

InputHandler::InputHandler()
{
    registerCallback(sf::Event::EventType::KeyPressed, std::bind(&InputHandler::onKeyPressed, 
                                                                this, 
                                                                std::placeholders::_1)); 
}

InputHandler::~InputHandler()
{

}

bool InputHandler::handle(const sf::Event& event) 
{
    auto it = mCallbackMap.find(event.type);
    
    if (it != mCallbackMap.end()) 
    {
        for (auto& cb : it->second) 
        {
            cb(event);
        }
    }
    
    return true; 
}

void InputHandler::registerCallback(sf::Event::EventType type, Callback callback) 
{
    mCallbackMap[type].push_back(callback);
}

void InputHandler::onKeyPressed(const sf::Event& anEvent)
{
    InputFrame frame; 

    if(sf::Keyboard::Up == anEvent.key.code)
    {
        frame.moveY -= 1; 
    }
    
    if(sf::Keyboard::Down == anEvent.key.code)
    {
        frame.moveY += 1; 
    }

    if(sf::Keyboard::Left == anEvent.key.code)
    {
        frame.moveX -= 1; 
    }

    if(sf::Keyboard::Right == anEvent.key.code)
    {
        frame.moveX += 1; 
    }

    mLatestInput = frame; 
}

InputFrame InputHandler::getLatestInput()
{
    InputFrame copy = mLatestInput; 
    InputFrame frame; 
    mLatestInput = frame; 
    
    return copy; 
}