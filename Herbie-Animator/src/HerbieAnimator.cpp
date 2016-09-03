#include "HerbieAnimator.h"

HerbieAnimator::HerbieAnimator()
{
    window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Herbie Animator");
    imageIcon.loadFromFile("data/icon.jpg");
    window->setIcon(imageIcon.getSize().x, imageIcon.getSize().y, imageIcon.getPixelsPtr());
    createHUD();
    scanner.readDir("test/", &vFrames, &screenWidth, &screenHeight);
    actFrame = 0;
    view = sf::View(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y));
}

void HerbieAnimator::setLeftHUDText(){
    if (leftHUDState == 0){
        leftHUD.setString("Press 'TAB' to show\n");
    }
    else if(leftHUDState == 1){
        leftHUD.setString(" **** --  Controls  -- **** \n"
                          "+----------+---------------+\n"
                          "| key      |    action     |\n"
                          "+----------+---------------+\n"
                          "| '<'      |previous frame |\n"
                          "| '>'      |next frame     |\n"
                          "| '-'      |decrease FPS   |\n"
                          "| '+'      |increase FPS   |\n"
                          "| 'p'      |pause          |\n"
                          "| 'o'      |open directory |\n"
                          "| 'wsad'   |move camera    |\n"
                          "| 'scroll' |zoom camera    |\n"
                          "| 'i'      |info           |\n"
                          "| 'v'      |visit webpage  |\n"
                          "| 'TAB'    |hide controls  |\n"
                          "+------+-------------------+");
    }
    else if (leftHUDState == 2){
        leftHUD.setString(" *** -- Info -- ***\n"
                          "+----------------------+\n"
                          "|    Herbie Animator   |\n"
                          "|        by stawrocek  |\n"
                          "|                      |\n"
                          "|[i] to hide           |\n"
                          "+----------------------+\n");
    }
}

void HerbieAnimator::setRightHUDText(){
    std::string strToWrite = "FPS: " + to_string<int>(fps) + " frame: (" + to_string<int>(actFrame+1) + "/" +
    to_string<int>(vFrames.size()) + ")";

    if(vFrames.size() == 0){
        strToWrite = "FPS: " + to_string<int>(fps) + " (no frames!) ";
    }

    if(paused)strToWrite += " (paused)";
        rightHUD.setString(strToWrite);

    double txtWidth = rightHUD.getLocalBounds().width*scaleFactor;
    rightHUD.setPosition(screenWidth - txtWidth - offset, offset);

    //rightHUD.setS
}

void HerbieAnimator::createHUD(){
    font.loadFromFile("data/VCR_OSD_MONO_1.ttf");

    leftHUD.setFont(font);
    leftHUD.setPosition(offset, offset);
    leftHUD.setStyle(sf::Text::Bold);
    leftHUD.setColor(sf::Color(135,42,42));
    leftHUD.setScale(scaleFactor, scaleFactor);
    setLeftHUDText();

    rightHUD.setFont(font);
    rightHUD.setStyle(sf::Text::Bold);
    rightHUD.setColor(sf::Color(135,42,42));
    rightHUD.setScale(scaleFactor, scaleFactor);
    setRightHUDText();
}

HerbieAnimator::~HerbieAnimator() {}

void HerbieAnimator::quit()
{
    window->close();
}

void HerbieAnimator::translateEvent(sf::Event event)
{
    if (event.type == sf::Event::TextEntered){
        if (event.text.unicode < 128){
            char input = static_cast<char>(event.text.unicode);
            if(input == '+'){
                fps++;
            }
            if(input == '-'){
                fps--;
            }
            if(input == '<'){
                actFrame--;
            }
            if(input == '>'){
                actFrame++;
            }
            fps = std::max(1, fps);
            actFrame = std::max(0, actFrame);
            actFrame = std::min((int)vFrames.size()-1, actFrame);
        }
    }
    if (event.type == sf::Event::Resized){
        view.reset(sf::FloatRect(0, 0, event.size.width, event.size.height));

        screenWidth = event.size.width;
        screenHeight = event.size.height;
        for(int i = 0; i < vFrames.size(); i++){
            vFrames[i]->sprite.setPosition(event.size.width/2.0, event.size.height/2.0);
        }
    }
    if (event.type == sf::Event::KeyReleased){
        if (event.key.code == sf::Keyboard::O){
            std::string toOpen = openDirectory(NULL);
            if(toOpen != "-1"){
                scanner.readDir(toOpen, &vFrames, &screenWidth, &screenHeight);
            }
        }
        if (event.key.code == sf::Keyboard::Tab){
            leftHUDState == 0 ? leftHUDState = 1 : leftHUDState = 0;
            setLeftHUDText();
        }
        if (event.key.code == sf::Keyboard::I){
            leftHUDState == 2 ? leftHUDState = 0 : leftHUDState = 2;
            setLeftHUDText();
        }
        if (event.key.code == sf::Keyboard::P){
            paused = !paused;
        }
        if (event.key.code == sf::Keyboard::V && leftHUDState == 1){
            launchGithub();
        }
    }
    if(event.type == sf::Event::MouseWheelMoved){
        view.zoom(1.f+event.mouseWheel.delta*0.1f);
    }
}

void HerbieAnimator::launchGithub(){
    #ifdef _WIN32
        system("start https://github.com/stawrocek/Herbie-Animator");
    #elif __APPLE__
        system("open https://github.com/stawrocek/Herbie-Animator");
    #elif __linux__
        std::string browser = getenv("BROWSER");
        std::string url = "https://github.com/stawrocek/Herbie-Animator";
        if(browser == "") return;
        char *args[3];
        args[0] = (char*)browser.c_str();
        args[1] = (char*)url.c_str();
        args[2] = 0;
        pid_t pid = fork();
        if(!pid)
        execvp(browser.c_str(), args);
    #endif
}

void HerbieAnimator::continuousEvents(float delta){
    float moveSpeed=750.0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        view.move(moveSpeed*delta, 0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        view.move(-moveSpeed*delta, 0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        view.move(0, moveSpeed*delta);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        view.move(0, -moveSpeed*delta);
    }
}

void HerbieAnimator::act(double deltaTime)
{
    if(!paused){
        actTime += deltaTime;
    }
    if(actTime > 1000000.0/fps)
    {
        actFrame++;
        if(actFrame >= vFrames.size())
        {
            actFrame=0;
        }
        actTime = 0;
    }
    setRightHUDText();
    scanner.update();
    if(openedPath != scanner.path){
        openedPath = scanner.path;
        window->setTitle(openedPath + " - Herbie Animator 1.0 by stawrocek");
    }
}

void HerbieAnimator::render()
{
    window->clear(sf::Color(0xb4, 0x8c,0x4f));

    window->setView(view);
    if(actFrame < vFrames.size())
    {
        window->draw(vFrames[actFrame]->sprite);
    }

    window->setView(sf::View(sf::FloatRect(0.f, 0.f, window->getSize().x, window->getSize().y)));
    window->draw(leftHUD);
    window->draw(rightHUD);
    window->display();
}

std::string HerbieAnimator::openDirectory(HWND owner = NULL)
{
    TCHAR szDir[MAX_PATH];
    BROWSEINFO bInfo;
    bInfo.hwndOwner = owner;
    bInfo.pidlRoot = NULL;
    bInfo.pszDisplayName = szDir;
    bInfo.lpszTitle = "Please, select a folder to track for animations";
    bInfo.ulFlags = 0;
    bInfo.lpfn = NULL;
    bInfo.lParam = 0;
    bInfo.iImage = -1;

    LPITEMIDLIST lpItem = SHBrowseForFolder( &bInfo);
    if( lpItem != NULL )
    {
      SHGetPathFromIDList(lpItem, szDir );
      return szDir;
    }
    return "-1";
}
