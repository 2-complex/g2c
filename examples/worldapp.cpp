
#include "worldapp.h"

#include "appenvironment.h"
#include "sprites.h"

#include "transforms.h"

using namespace g2c;

WorldApp::WorldApp() {}
WorldApp::~WorldApp() {}

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
    glewInit();

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

