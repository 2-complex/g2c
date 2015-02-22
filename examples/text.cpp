
#include "sprites.h"
#include "app.h"
#include "launch.h"
#include "transforms.h"

using namespace g2c;

class TextApp : public App
{
    Bank* bank;
    Font font;
    Text text;
    RendererGL2 renderer;

    void init();
    void resize(int width, int height);
    void setBank(Bank* bank);

    void draw() const;
};

void TextApp::init()
{
    Bitmap bitmap;
    bank->initBitmapWithPath(&bitmap, "helv.png");
    font.initWithBitmap(bitmap);
    font.numberOfRows = 6;
    font.numberOfColumns = 16;

    font.getWidthsFromBitmap(bitmap);

    text.font = &font;
    text.s = "My Message";
    text.x = 100;
    text.y = 100;

    renderer.init();
    Mesh::renderer = &renderer;

    glDisable(GL_DEPTH_TEST);
}

void TextApp::setBank(Bank* bank)
{
    this->bank = bank;
}

void TextApp::resize(int width, int height)
{
    renderer.projection = orthographic(0,width,0,height,-1,1);
}

void TextApp::draw() const
{
    text.draw();
}

LAUNCH(TextApp)