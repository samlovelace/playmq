#ifndef IGAMESTATE_HPP
#define IGAMESTATE_HPP
 
#include <SFML/Graphics.hpp>
 
class IGameState 
{ 
public:
    ~IGameState();

    virtual void handleInput(sf::Event& anEvent) = 0; 
    virtual void update(float dt) = 0; 
    virtual void render(sf::RenderWindow& aWindow) = 0; 

private:
   
};
#endif //IGAMESTATE_HPP
