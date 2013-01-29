
#include "spriteenvironment.h"
#include "macbank.h"

int main()
{
	SpriteEnvironment se;
	
	Sprite ship;
	MacFileSystemBank bank;
	bank.initTextureWithPath(&ship, "ship.png");
	
	Actor enterprise;
	enterprise.sprite = &ship;
	//se.world->add(&enterprise);
	
	se.mainLoop();
}

