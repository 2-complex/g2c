
#include "spriteenvironment.h"

class ShipEnvironment : public SpriteEnvironment {
	Sprite ship;
	Actor enterprise;
	
	void init();
	void draw() const;
};

void ShipEnvironment::init()
{
	bank.initTextureWithPath(&ship, "ship.png");
	ship.numberOfRows = 2;
	ship.numberOfColumns = 32;
	
	enterprise.sprite = &ship;
	enterprise.x = 100;
	enterprise.y = 200;
	enterprise.frame = 7;
}

void ShipEnvironment::draw() const
{
	enterprise.draw();
}

int main(int argc, char** args)
{
	ShipEnvironment e;
	e.mainLoop();
	
	return 0;
}

