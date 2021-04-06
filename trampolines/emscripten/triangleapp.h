#ifndef _TRIANGLE_APP_
#define _TRIANGLE_APP_

#include "graphics.h"
#include "app.h"

using namespace cello;

class TriangleApp : public App
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
