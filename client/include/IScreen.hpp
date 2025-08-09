#ifndef ISCREEN_HPP
#define ISCREEN_HPP
 
#include <SFML/Graphics.hpp>
 
class IScreen 
{ 
public:
    ~IScreen() = default; 

    virtual bool handleInput(sf::Event& anEvent) = 0; 
    virtual bool update() = 0; 
    virtual bool draw(sf::RenderWindow& aWindow) = 0; 

private:
   
};
#endif //ISCREEN_HPP