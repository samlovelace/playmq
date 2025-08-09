
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

            mScreens.at(mActiveScreen)->handleInput(e);  
        }

        mScreens.at(mActiveScreen)->update(); 

        // the active screen is responsible for calling mWindow.clear() if desired 
        mScreens.at(mActiveScreen)->draw(mWindow); 
        
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