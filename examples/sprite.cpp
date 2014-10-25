
#include "spriteenvironment.h"
#include "launch.h"

class ShipApp : public App
{
	Sprite ship;
	Actor enterprise;
	Bank* bank;
	RendererGL2 renderer;

	void init();
	void draw() const;

	void setBank(Bank* bank);
};


void ShipApp::setBank(Bank* bank)
{
	this->bank = bank;
}


void ShipApp::init()
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        renderer.init();

	Sprite::renderer = &renderer;

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

