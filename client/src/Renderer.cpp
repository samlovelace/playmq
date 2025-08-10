
#include "Renderer.h"
#include "MenuScreen.h"
#include "TankGameScreen.h"

Renderer::Renderer(std::shared_ptr<InputHandler> aHandler) : mActiveScreen(1), mInputHandler(aHandler)
{
    mScreens.push_back(std::make_unique<MenuScreen>());
    mScreens.push_back(std::make_unique<TankGameScreen>()); 
}

Renderer::~Renderer()
{

}

void Renderer::run()
{
    mWindow.create(sf::VideoMode(500, 500), "playmq");
    
    while(mWindow.isOpen())
    {
        sf::Event e; 
        while(mWindow.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                mWindow.close(); 
            }

            mInputHandler->handle(e); 
        }

        mWindow.clear(sf::Color::Black); 
        updateLatestGameState(); 
        render(); 
        mWindow.display(); 
    } 
}

void Renderer::setAvailableGames(nlohmann::json& aSetOfGames)
{
    std::vector<std::string> games; 

    for(const auto& game : aSetOfGames["games"])
    {
        games.push_back(game); 
    }

    static_cast<MenuScreen*>(mScreens.at(0).get())->setGames(games);
}

void Renderer::updateLatestGameState()
{
    for(const auto& player : mLatestGameState)
    {

        // TODO: is there a better spot to put this? I feel like i need to check it every update loop
        if(mPlayerShapesMap.find(player.id) == mPlayerShapesMap.end())
        {
            // player isnt in the map, add the player 
            sf::RectangleShape newPlayer = sf::RectangleShape(sf::Vector2f(10, 10));
            
            // TODO: determine this on the server side, add to playerState? Doesnt need to be sent every time though
            newPlayer.setFillColor(sf::Color::Blue); 

            mPlayerShapesMap.insert({player.id, newPlayer}); 
        }

        mPlayerShapesMap[player.id].setPosition(sf::Vector2f(player.x, player.y)); 
    }
}

void Renderer::render()
{
    for(const auto& [id, playerShape] : mPlayerShapesMap)
    {
        mWindow.draw(playerShape); 
    }
}