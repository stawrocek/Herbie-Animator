#include <SFML/Graphics.hpp>

#include "HerbieAnimator.h"

int main()
{
    HerbieAnimator herbie;
    sf::Clock actClock;
    bool continueLoop=true;
    while (continueLoop)
    {
        sf::Event event;
        while (herbie.window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                herbie.window->close();
                continueLoop=false;
                break;
            }
            herbie.translateEvent(event);
        }
        herbie.continuousEvents();
        herbie.act(actClock.getElapsedTime().asMicroseconds());
        actClock.restart();
        herbie.render();
    }
    return 0;
}

