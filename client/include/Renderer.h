#ifndef RENDERER_H
#define RENDERER_H
 
#include <SFML/Graphics.hpp>
#include <memory>
#include "IScreen.hpp"
#include <nlohmann/json.hpp>
#include "InputHandler.h"

class Renderer 
{ 
public:
    Renderer(std::shared_ptr<InputHandler> aHandler);
    ~Renderer();

    void run(); 

    void setAvailableGames(nlohmann::json& aMapOfGames); 

private:

    sf::RenderWindow mWindow; 
    std::shared_ptr<InputHandler> mInputHandler; 

    int mActiveScreen; 
    std::vector<std::unique_ptr<IScreen>> mScreens; 

};
#endif //RENDERER_H