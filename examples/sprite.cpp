
#include "spriteenvironment.h"
#include "launch.h"
#include "transforms.h"

class ShipApp : public App
{
    Sprite ship;
    Actor enterprise;
    Bank* bank;
    RendererGL2 renderer;

    void init();
    void reshape(int width, int height);
    void draw() const;

    void setBank(Bank* bank);
};

void ShipApp::setBank(Bank* bank)
{
    this->bank = bank;
}

void ShipApp::reshape(int width, int height)
{
    renderer.projection = orthographic(0,width,0,height,-1,1);
}

void ShipApp::init()
{
    Sprite::renderer = &renderer;
    renderer.init();

    bank->initTextureWithPath(&ship, "ship.png");
    ship.numberOfRows = 2;
    ship.numberOfColumns = 32;

    enterprise.sprite = &ship;
    enterprise.x = 100;
    enterprise.y = 200;
    enterprise.frame = 7;
}

void ShipApp::draw() const
{
    enterprise.draw();
}

int main(int argc, char** args)
{
    ShipApp app;
    launch(&app);
    return 0;
}

