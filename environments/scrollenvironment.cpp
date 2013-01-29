
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


#include "scrollenvironment.h"

#include "util.h"
#include "lin/lin.h"


ScrollEnvironment::ScrollEnvironment() {}
ScrollEnvironment::~ScrollEnvironment() {}

void ScrollEnvironment::display()
{
	if(!valid || animate)
	{
		compute();
		valid = true;
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
	glTranslatef(center.x, center.y, 0.0);
    
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    draw();
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glutSwapBuffers();
    if(animate)
    	glutPostRedisplay();
}

void ScrollEnvironment::motion(int x, int y)
{
	Vec2 c = flip(Vec2(x,y));
	
	if( dragging )
	{
		center += c - last;
		last = c;
	}
	
	mouseDragged(c);
}

Vec2 ScrollEnvironment::getCenter() const
{
	return center;
}

void ScrollEnvironment::setCenter(double x, double y)
{
	center.set(x,y);
}

