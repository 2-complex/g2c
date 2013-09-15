
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


#ifndef _APP_
#define _APP_

#include "lin/lin.h"

namespace g2c {




class App {
public:
    App();
    virtual ~App();
	
    virtual void init();
    virtual void step(double t);
    virtual void draw() const;
    
	virtual bool mouseDown(const Vec2& C);
	virtual void mouseDragged(const Vec2& C);
	virtual void mouseUp(const Vec2& C);
	
	virtual bool touchDown(unsigned int index, const Vec2& C);
	virtual void touchDragged(unsigned int index, const Vec2& C);
	virtual void touchUp(unsigned int index, const Vec2& C);
	
	virtual void controllerKeyDown(int deviceId, char c);
	virtual void controllerKeyUp(int deviceId, char c);
	virtual void controllerMotion(int deviceId, int inputId, double x, double y);
    
    virtual void keyboard(unsigned char inkey);
    virtual void special(int inkey);
    
    virtual void reshape(int width, int height);
    
    bool getNeedsKeyboard();
	void finishTyping();

protected:
	void showKeyboard();
	void hideKeyboard();

private:
	bool needsKeyboard;
};

} // end namespace


#endif

