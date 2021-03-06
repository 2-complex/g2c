
#include "sprites.h"
#include "app.h"
#include "launch.h"
#include "transforms.h"

using namespace cello;

class BitmapApp : public App
{
    void draw() const;
    void init();
    void setBank(Bank* bank);
    void resize(int width, int height);

    Bitmap bitmap;
    Sprite ladybug;
    Actor cynthia;
    RendererGL2 renderer;

    Bank* bank;
};

void BitmapApp::setBank(Bank* bank)
{
    this->bank = bank;
}

void BitmapApp::resize(int width, int height)
{
    renderer.projection = orthographic(0,width,0,height,-1,1);
}

void BitmapApp::init()
{
    bank->initBitmapWithPath(&bitmap, "assets/ladybug.png");
    ladybug.initWithBitmap(bitmap);
    cynthia.sprite = &ladybug;

    renderer.init();
    Mesh::renderer = &renderer;
}

void BitmapApp::draw() const
{
    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    cynthia.draw();
}

int main()
{
    BitmapApp app;
    launch(&app);
    return 0;
}

