#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H
 
#include <SFML/Graphics.hpp>
 
using Callback = std::function<void(const sf::Event&)>; 

class InputHandler 
{ 
public:
    InputHandler();
    ~InputHandler();

    bool handle(const sf::Event& anEvent); 
    void registerCallback(sf::Event::EventType type, Callback callback);

private:

    std::map<sf::Event::EventType, std::vector<Callback>> mCallbackMap; 

    /****** Callbacks *********/
    void onKeyPressed(const sf::Event& anEvent);
    void onMouseClicked(const sf::Event& anEvent); 

   
};
#endif //INPUTHANDLER_H