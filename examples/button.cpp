
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
    Layer layer;

    void init();
    void resize(int width, int height);
    void setBank(Bank* bank);

    bool mouseDown(const Vec2& C);
    void mouseDragged(const Vec2& C);
    void mouseUp(const Vec2& C);
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
    bank->initTextureWithPath(&button, "assets/button.png");
    button.numberOfRows = 1;
    button.numberOfColumns = 2;

    onoff.sprite = &button;
    onoff.x = 100;
    onoff.y = 200;
    onoff.frame = 7;

    onoff.handler = &handler;

    renderer.init();
    Mesh::renderer = &renderer;

    layer.add(&onoff);
}

bool ButtonApp::mouseDown(const Vec2& C)
{
    return layer.mouseDown(C);
}

void ButtonApp::mouseDragged(const Vec2& C)
{
    layer.mouseDragged(C);
}

void ButtonApp::mouseUp(const Vec2& C)
{
    layer.mouseUp(C);
}

void ButtonApp::draw() const
{
    layer.draw();
}

int main(int argc, char** args)
{
    ButtonApp app;
    launch(&app);

    return 0;
}

