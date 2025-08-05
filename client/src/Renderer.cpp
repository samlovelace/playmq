
#include "Renderer.h"

Renderer::Renderer()
{
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
        }

        mWindow.clear(sf::Color::Blue); 

        //mWindow.draw(); 

        mWindow.display(); 
    } 


}