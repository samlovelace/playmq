
#include "TankGameScreen.h"

TankGameScreen::TankGameScreen() : mVelocity(sf::Vector2f(0, 0)), mPosition(sf::Vector2f(10, 10)), mSpeed(4), 
    mShape(sf::Vector2f(50, 50))
{ 
    mShape.setFillColor(sf::Color::Blue); 

}

TankGameScreen::~TankGameScreen()
{

}

bool TankGameScreen::handleInput(sf::Event& anEvent)
{
    mVelocity.x = 0; 
    mVelocity.y = 0; 

    if(sf::Event::KeyPressed == anEvent.type)
    {
        if(sf::Keyboard::Up == anEvent.key.code)
        {
            mVelocity.y -= mSpeed; 
        }
        
        if(sf::Keyboard::Down == anEvent.key.code)
        {
            mVelocity.y += mSpeed; 
        }

        if(sf::Keyboard::Left == anEvent.key.code)
        {
            mVelocity.x -= mSpeed; 
        }

        if(sf::Keyboard::Right == anEvent.key.code)
        {
            mVelocity.x += mSpeed; 
        }
    }

}

bool TankGameScreen::update()
{
    // TODO: make dt configurable 
    mPosition.x += mVelocity.x * 0.01;
    mPosition.y += mVelocity.y * 0.01;
    
    mShape.setPosition(mPosition); 
}

bool TankGameScreen::draw(sf::RenderWindow& aWindow)
{
    aWindow.clear(sf::Color::Black); 
    aWindow.draw(mShape); 
}