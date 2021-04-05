#include "triangleapp.h"

#include "graphics.h"
#include "app.h"

using namespace std;
using namespace g2c;

PanApp::PanApp()
    : cameraRadius(10)
    , cameraPhi(.5)
    , cameraTheta(-0.25)
    , firstX(0)
    , firstY(0)
    , firstTheta(0)
    , firstPhi(0)
{
}

PanApp::~PanApp()
{
}

bool PanApp::mouseDown(const Vec2& C)
{
    last = C;
    return true;
}

void PanApp::mouseDragged(const Vec2& C)
{
    Vec2 delta = -0.01*(C - last);
    cameraTheta += delta.x;
    cameraPhi += delta.y;

    if(cameraPhi > 0.5*M_PI) cameraPhi = 0.5*M_PI;
    if(cameraPhi < -0.5*M_PI) cameraPhi = -0.5*M_PI;

    last = C;
}


void PanApp::keyboard(unsigned char inkey)
{
    switch(inkey)
    {
        case '+':
        case '=':
            cameraRadius *= 0.9;
        break;

        case '_':
        case '-':
            cameraRadius /= 0.9;
        break;

        default:
        break;
    }
}

void PanApp::resize(int width, int height)
{
    projection = perspective(
        20.0, /* field of view in degree */
        1.0 * width / height, /* aspect ratio */
        0.1 * cameraRadius, /* Z near */
        1000 * cameraRadius ); /* Z far */
}

Vec3 PanApp::getCameraLoc() const
{
    return cameraLookAtLoc
        + cameraRadius * Vec3(
            cos(cameraTheta)*cos(cameraPhi),
            sin(cameraTheta)*cos(cameraPhi),
            sin(cameraPhi));
}

void PanApp::setLookAtLoc(const Vec3& V)
{
    cameraLookAtLoc = V;
}

Vec3 PanApp::getLookAtLoc() const
{
    return cameraLookAtLoc;
}

Mat4 PanApp::getProjection() const
{
    return projection;
}

void TriangleApp::init()
{
    effect.vertexCode =
        "attribute vec4 position;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vec4(position.xyz, 1.0);\n"
        "}\n";

    effect.fragmentCode =
        "precision mediump float;\n"
        "void main()\n"
        "{\n"
        "  gl_FragColor[0] = gl_FragCoord.x/640.0;\n"
        "  gl_FragColor[1] = gl_FragCoord.y/480.0;\n"
        "  gl_FragColor[2] = 0.5;\n"
        "}\n";

    effect.compile();

    float vertexArray[] =
    {
        0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f
    };

    int indexArray[] = { 0, 1, 2 };

    buffer.set(vertexArray, sizeof(vertexArray) / sizeof(float));
    indexBuffer.set(indexArray, sizeof(indexArray) / sizeof(int));

    field = Field(&buffer, 2, 2, 0);
    geometry["position"] = field;
    geometry.indices = &indexBuffer;

    material.effect = &effect;

    shape.geometry = &geometry;
    shape.assumptions.push_back(&camera);
    shape.assumptions.push_back(&material);

    material["color"] = Vec4(1.0, 0.0, 0.0, 1.0);
}

void TriangleApp::draw() const
{
    shape.draw();
}
