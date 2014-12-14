
#include "app.h"
#include "sprites.h"
#include "launch.h"
#include "transforms.h"

using namespace g2c;

class MyButtonHandler : public ButtonHandler
{
    void click(Button* button);
};

void MyButtonHandler::click(Button* button)
{
    g2clog( "I got clicked... or tapped.\n" );
}

class ButtonApp : public App
{
    Sprite button;
    Button onoff;
    MyButtonHandler handler;
    Bank* bank;
    RendererGL2 renderer;

    void init();
    void resize(int width, int height);
    void setBank(Bank* bank);

    void draw() const;
};

void ButtonApp::setBank(Bank* bank)
{
    this->bank = bank;
}

void ButtonApp::resize(int width, int height)
{
    renderer.projection = orthographic(0,width,0,height,-1,1);
}

void ButtonApp::init()
{
    bank->initTextureWithPath(&button, "button.png");
    button.numberOfRows = 1;
    button.numberOfColumns = 2;

    onoff.sprite = &button;
    onoff.x = 100;
    onoff.y = 200;
    onoff.frame = 7;

    onoff.handler = &handler;

    renderer.init();
    Mesh::renderer = &renderer;
}


void ButtonApp::draw() const
{
    onoff.draw();
}

int main(int argc, char** args)
{
    ButtonApp app;
    launch(&app);

    return 0;
}

