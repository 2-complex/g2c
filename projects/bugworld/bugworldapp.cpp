
#include "bugworldapp.h"

#include "transforms.h"

#include "bank.h"
#include "player.h"

using namespace g2c;


void BugWorldApp::setBank(Bank* inBank)
{
	this->bank = inBank;
}

void BugWorldApp::reshape(int width, int height)
{
	renderer->projection = orthographic(0, width, 0, height, -1, 1);
}

void BugWorldApp::init()
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

void BugWorldApp::draw() const
{
	world->draw();
}

void BugWorldApp::destroy()
{
	delete world;
	delete renderer;
}

