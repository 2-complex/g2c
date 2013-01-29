
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


#ifndef _ENVIRONMENT_
#define _ENVIRONMENT_

#include "listener.h"
#include "opengl.h"
#include "util.h"
#include "lin/lin.h"

class Environment : public Listener {
friend void fdisplay();
friend void freshape(int w, int h);
friend void fidle();

public:
	Environment();
	virtual ~Environment();

protected:
	/*	window height and width updated in the reshape function*/
	double windowHeight;
	double windowWidth;

    /*	valid is a flag that gets set using the function invalidate() when
	    the scene needs to recompute.*/
	mutable bool valid;
	
	/*	dragging is a variable for use by the click-drag mechanism for moving the
	    camera.  When the flag gets set in button, the subsequent mouse-drag and
	    mouse-up events are interpreted to pan the camera.*/
	bool dragging;
	
	/*	While the mouse is getting dragged, this is the location of the previous
		mouse event*/
	Vec2 last;
	
	
	/*	display gets called by the glut display function.  Think twice before
		overriding if draw can do the job.  display calls compute, so it is
		not const.*/
	virtual void display();
	
	/*	button gets called by the like-named glut function.
		Think twice before overriding if the same job could be done with mouseDown
		mouseDragged and mouseUp.*/
	virtual void button( int b, int state, int x, int y );
	
	/*	motion gets called by the like-named glut function.
		Think twice before overriding if the same job could be done with mouseDown
		mouseDragged and mouseUp.*/
	virtual void motion( int x, int y );
    
	/*  reshape gets called directly by the glut reshape function.*/
    virtual void reshape( int w, int h );
	
	/*	idle gets called by the glut idle function.*/
	virtual void idle();
	
	/*	flip reports the screen location of the point given coordinates of the
	    form that gl button etc reports them.*/
	Vec2 flip(const Vec2& v) const;
	
	/*	compute() is called by display() when the valid flag is set false.
	    Override compute() to do work to compute what to draw.  Call
	    invalidate() to set the valid flag to false, so that compute calls.*/
	virtual void compute();
	
	/*	draw is called after a clear and inside a matrix push.  Override draw()
 	    to add graphics to the scene using ordinary gl calls like glVertex3f().*/
	virtual void draw() const;
	
	/*	sets the valid flag to false to indicate that the scene should recompute
	    (not redraw, recompute)*/
	void invalidate() const;
    
    /*  gets called by initGlut.  Override to turn things on and off, or set the
        clear-color blending or whathaveyou*/
    virtual void enables();
    
public:
	/*	If true, calls display over and over*/
	bool animate;
	
	/*	Accessors for modelView and porjection matrices*/
    Mat4 getModelView() const;
    Mat4 getProjection() const;
    
    /*  initGlut Initilaizes glut.  Sets up the gl context, sets flags etc.
        Must be called before the mainLoop.*/
    void initGlut(const char* windowName="",
                  int windowSizeX=640,
                  int windowSizeY=480);
    
    /*  Call this to start the glut machine.*/
    void mainLoop();
    
private:
	bool glutInitted;
};


#endif