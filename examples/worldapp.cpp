
#include "sprites.h"
#include "transforms.h"
#include "rendererg.h"
#include "launch.h"


using namespace g2c;
using namespace std;

class WorldApp : public App
{
public:
    WorldApp();
    virtual ~WorldApp();

    string filename;

    virtual void setBank(Bank* inBank);

    virtual void init();
    virtual void resize(int width, int height);
    virtual void draw() const;
    virtual void destroy();

private:
    Bank* bank;
    World* world;
    Renderer* renderer;
};

WorldApp::WorldApp()
    : filename("assets/bug.world")
{
}

WorldApp::~WorldApp()
{
}

void WorldApp::setBank(Bank* inBank)
{
    this->bank = inBank;
}

void WorldApp::resize(int width, int height)
{
    renderer->projection = orthographic(0, width, 0, height, -1, 1);
}

void WorldApp::init()
{
    world = new World;
    renderer =  new RendererG;
    renderer->init();

    Mesh::renderer = renderer;

    world->bank = bank;
    bank->initSerializableWithPath(world, filename.c_str());
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
    WorldApp app;

    if( argc > 1 )
        app.filename = args[1];

    launch(&app);
    return 0;
}


