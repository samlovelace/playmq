
#include "MenuScreen.h"
#include <iostream> 

MenuScreen::MenuScreen()
{

}

MenuScreen::~MenuScreen()
{

}

bool MenuScreen::update()
{
    if(!mGameNames.empty())
    {

    }
}

bool MenuScreen::handleInput(sf::Event& anEvent)
{
    // check which button was clicked or something to select game 
}

bool MenuScreen::draw(sf::RenderWindow& aWindow)
{
    aWindow.clear(sf::Color::Black); 

    sf::Vector2u windowSize = aWindow.getSize(); 
    unsigned int width = windowSize.x; 
    unsigned int height = windowSize.y; 

    for(int i = 0; i < mGameNames.size(); i++)
    {
        sf::RectangleShape option = sf::RectangleShape(sf::Vector2f(150, 50)); 
        
        option.setPosition(sf::Vector2f(width/3, 0.25 * height + i * (0.5 * height) / mGameNames.size()));
        option.setOutlineColor(sf::Color::Red); 
        option.setFillColor(sf::Color::Blue); 
        
        aWindow.draw(option); 
    }
}

void MenuScreen::setGames(const std::vector<std::string>& aGamesVec)
{
    mGameNames.clear(); 
    mGameNames = aGamesVec; 
}