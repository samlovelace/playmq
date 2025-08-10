#ifndef RENDERER_H
#define RENDERER_H
 
#include <SFML/Graphics.hpp>
#include <memory>
#include "IScreen.hpp"
#include <nlohmann/json.hpp>
#include "InputHandler.h"
#include "PlayerState.hpp"
#include <mutex> 

class Renderer 
{ 
public:
    Renderer(std::shared_ptr<InputHandler> aHandler);
    ~Renderer();

    void run(); 

    void setAvailableGames(nlohmann::json& aMapOfGames); 
    void setLatestGameState(const std::vector<PlayerState> aGameState) {std::lock_guard<std::mutex> lock(mGameStateMtx); mLatestGameState = aGameState;}

private:

    sf::RenderWindow mWindow; 
    std::shared_ptr<InputHandler> mInputHandler; 

    int mActiveScreen; 
    std::vector<std::unique_ptr<IScreen>> mScreens; 

    std::mutex mGameStateMtx; 
    std::vector<PlayerState> mLatestGameState; 

    void updateLatestGameState();
    void render(); 

    bool isPlayerKnown(int aPlayerId);
    void addPlayer(int aPlayerId); 

    std::map<int, sf::RectangleShape> mPlayerShapesMap; 

};
#endif //RENDERER_H