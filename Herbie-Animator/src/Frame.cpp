#include "Frame.h"

Frame::Frame(){}

Frame::~Frame(){

}

Frame::Frame(std::string _texPath, double * width, double * height)
:screenWidth(width), screenHeight(height)
{
    texPath = _texPath;
    if(!texture.loadFromFile(texPath)){
        std::cout << "[! error] could not load " + texPath + " as animation's frame\n";
    }
    sprite.setTexture(texture);
    sprite.setOrigin(sprite.getGlobalBounds().width/2.f, sprite.getGlobalBounds().height/2.f);
    sprite.setPosition(*screenWidth/2.0, *screenHeight/2.0);
}

bool Frame::reload(){
    return texture.loadFromFile(texPath);
}
