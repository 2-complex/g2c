
/*
  g2c Copyright (C) 2012 2-Complex

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/


#ifndef _SPRITE_ENVIRONMENT_
#define _SPRITE_ENVIRONMENT_

#include "sprites.h"
#include "scrollenvironment.h"

#include <string>

using namespace sprite;

class SpriteEnvironment : public ScrollEnvironment {
public:
	SpriteEnvironment();
	virtual ~SpriteEnvironment();
	
	sprite::World* world;
	sprite::Renderer* renderer;
	
	std::string filename;
	bool forwardKeyboard;
	
	void initWithPath(const char* filename);
	
	virtual void enables();
	virtual void draw() const;
	
	virtual bool mouseDown(const Vec2& C);
	virtual void mouseDragged(const Vec2& C);
	virtual void mouseUp(const Vec2& C);
	
	virtual void keyboard(unsigned char inkey);
	
private:
	int sound_index;
	sprite::World* defaultWorld;
	sprite::Actor* current;
	Vec2 lastLoc;
};

#endif

