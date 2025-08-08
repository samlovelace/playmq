
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
    if (anEvent.key.code == sf::Keyboard::Escape)
        std::cout << "Escape pressed!\n";
}