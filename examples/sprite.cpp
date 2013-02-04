
#include "spriteenvironment.h"
#include "macbank.h"

using namespace sprite;

#include <stdio.h>


int main(int argc, char** args)
{
	SpriteEnvironment environment;
	
    environment.initGlut("My Window", 1400, 900);
   	
   	MacFileSystemBank bank;
   	Sprite ship;
   	Actor actor;
   	
   	bank.initTextureWithPath(&ship, "ship.png");
   	ship.numberOfRows = 2;
   	ship.numberOfColumns = 32;
   	ship.center = true;
   	
   	actor.sprite = &ship;
   	actor.x = 100;
   	actor.y = 100;

    environment.world->add(&actor);
    
    environment.mainLoop();
}

