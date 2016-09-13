#ifndef HERBIEANIMATOR_H
#define HERBIEANIMATOR_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

#include "Frame.h"
#include "DirScanner.h"

#include <windows.h>
#include <shlobj.h>

class HerbieAnimator
{
    public:
        HerbieAnimator();
        virtual ~HerbieAnimator();

        void quit();
        void translateEvent(sf::Event ev);
        void render();
        void act(double deltaTime);
        void addTexture(std::string path);
        std::string openDirectory(HWND hwnd);
        void continuousEvents(float delta);
        void createHUD();

        void setLeftHUDText();
        void setRightHUDText();
        void launchGithub();

        sf::RenderWindow * window;

        double screenWidth=800;
        double screenHeight=600;

        sf::Texture backgroundTexture;
        sf::Sprite backgroundSprite;
        sf::Font font;
        sf::Text rightHUD;
        sf::Text leftHUD;
        float offset = 5;
        float scaleFactor=0.7;

    private:
        int fps=5.0f;
        std::string path;                       //path to scaned dir
        double actTime=0;                       //in millis
        bool paused=false;
        std::vector<Frame*> vFrames;
        sf::Image imageIcon;
        sf::View view;
        int actFrame;
        DirScanner scanner;
        std::string openedPath;

        int leftHUDState=0;
};

//fcuk gcc 4.7.1
template < typename T > std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}



#endif // HERBIEANIMATOR_H
