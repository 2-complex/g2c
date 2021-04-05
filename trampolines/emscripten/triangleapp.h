#ifndef _TRIANGLE_APP_
#define _TRIANGLE_APP_

#include "graphics.h"
#include "app.h"

using namespace g2c;

class PanApp : public App
{
protected:
    PanApp();
    virtual ~PanApp();

private:
    double cameraTheta;
    double cameraPhi;
    double cameraRadius;
    Vec3 cameraLookAtLoc;
    int firstX;
    int firstY;
    double firstTheta;
    double firstPhi;
    Vec2 last;
    Mat4 projection;

    bool mouseDown(const Vec2& C);
    void mouseDragged(const Vec2& C);
    void keyboard(unsigned char inkey);
    void resize(int width, int height);

protected:
    Vec3 getCameraLoc() const;
    Vec3 getLookAtLoc() const;
    void setLookAtLoc(const Vec3& V);
    Mat4 getProjection() const;
};

class TriangleApp : public PanApp
{
public:
    Buffer buffer;
    IndexBuffer indexBuffer;
    Geometry geometry;
    mutable Effect effect;
    Assumption material;
    mutable Assumption camera;
    Field field;
    Shape shape;

    void init();
    void draw() const;
};

#endif
