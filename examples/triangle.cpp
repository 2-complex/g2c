
#include "environment.h"
#include "brafix.h"

class MyEnvironment : public Environment {
protected:
	void draw() const;
};

void MyEnvironment::draw() const
{
	glColor3f(0,0,1);
	glBegin(GL_POLYGON);
	glVertex3f(0,0,0);
	glVertex3f(1,0,0);
	glVertex3f(0,1,0);
	glEnd();
}

int main()
{
	MyEnvironment e;
	e.mainLoop();
	
	return 0;
}
