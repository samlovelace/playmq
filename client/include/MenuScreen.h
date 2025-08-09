#ifndef MENUSCREEN_H
#define MENUSCREEN_H
 
#include "IScreen.hpp" 

class MenuScreen : public IScreen
{ 
public:
    MenuScreen();
    ~MenuScreen(); 

    bool handleInput(sf::Event& anEvent) override; 
    bool update() override; 
    bool draw(sf::RenderWindow& aWindow) override; 

    void setGames(const std::vector<std::string>& aGamesVec); 

private:

    std::vector<std::string> mGameNames; 
   
};
#endif //MENUSCREEN_H