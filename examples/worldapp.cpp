
#include "appenvironment.h"
#include "sprites.h"
#include "macbank.h"
#include "openalplayer.h"

#include "transforms.h"

using namespace g2c;


class WorldApp : public App
{
public:
    virtual void setBank(Bank* inBank);

    virtual void init();
    virtual void reshape(int width, int height);
    virtual void draw() const;
    virtual void destroy();

private:
    Bank* bank;
    World* world;
    Renderer* renderer;
};


void WorldApp::setBank(Bank* inBank)
{
	this->bank = inBank;
}

void WorldApp::reshape(int width, int height)
{
	renderer->projection = orthographic(0, width, 0, height, -1, 1);
}

void WorldApp::init()
{
    world = new World;
    renderer =  new RendererGL2;
    renderer->init();

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    Sprite::renderer = renderer;

    world->bank = bank;
    bank->initSerializableWithPath(world, "bug.world");
    g2clog( "%s\n", world->serialize().c_str() );
}


void WorldApp::draw() const
{
	world->draw();
}


void WorldApp::destroy()
{
	delete world;
	delete renderer;
}


int main(int argc, char** args)
{
    AppEnvironment environment;
    WorldApp app;

    environment.initWindow("World", 800, 600);

    MacFileSystemBank* bank = new MacFileSystemBank;
    OpenALPlayer* player = new OpenALPlayer;

    app.setBank(bank);
    app.setPlayer(player);

    environment.app = &app;

    environment.mainLoop();

    delete bank;
    delete player;

    return 0;
}

