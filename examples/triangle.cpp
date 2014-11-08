
#include "app.h"
#include "launch.h"

using namespace g2c;

<<<<<<< HEAD
class MyEnvironment : public Environment
=======
class TriangleApp : public App
>>>>>>> fb5f9c21ae1c7ebc2990a93eb75b69458460a31f
{
protected:
    void draw() const;
};

void TriangleApp::draw() const
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
<<<<<<< HEAD
	MyEnvironment e;
	e.mainLoop();

	return 0;
=======
    TriangleApp app;
    launch(&app);
    return 0;
>>>>>>> fb5f9c21ae1c7ebc2990a93eb75b69458460a31f
}
