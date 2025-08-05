#ifndef RENDERER_H
#define RENDERER_H
 
#include <SFML/Graphics.hpp>

class Renderer 
{ 
public:
    Renderer();
    ~Renderer();

    void run(); 

private:

    sf::RenderWindow mWindow; 
   
};
#endif //RENDERER_H