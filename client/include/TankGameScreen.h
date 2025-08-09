#ifndef TANKGAMESCREEN_H
#define TANKGAMESCREEN_H
 
#include "IScreen.hpp"
 
class TankGameScreen : public IScreen
{ 
public:
    TankGameScreen();
    ~TankGameScreen();

    bool handleInput(sf::Event& anEvent) override; 
    bool update() override; 
    bool draw(sf::RenderWindow& aWindow) override; 

private:

    float mSpeed; 
    sf::Vector2f mPosition; 
    sf::Vector2f mVelocity; 

    sf::RectangleShape mShape; 


};
#endif //TANKGAMESCREEN_H