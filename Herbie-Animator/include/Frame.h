#ifndef FRAME_H
#define FRAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Frame
{
    public:
        Frame();
        Frame(std::string texPath, double * width, double * height);
        virtual ~Frame();

        bool reload();
        std::string texPath;
        sf::Texture texture;
        sf::Sprite  sprite;

        double * screenWidth;
        double * screenHeight;
};

#endif // FRAME_H
