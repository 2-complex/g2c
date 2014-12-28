
#include "panenvironment.h"
#include "graphics.h"

#if __APPLE_CC__
#include "macbank.h"
#else
#include "unixbank.h"
#endif

#include <stdio.h>

using namespace g2c;


class ModelApp : public App
{
public:
    ModelApp();
    virtual ~ModelApp();

    Model* model;
    std::string filename;

    void setCenter(const Vec3& V);
    void setBank(Bank* inBank);

    void init();
    void draw() const;

protected:
    Bank* bank;

    virtual void reshape(int width, int height);
    virtual void draw() const;
    virtual bool mouseDown(const Vec2& C);
    virtual void mouseDragged(const Vec2& C);
    virtual void mouseUp(const Vec2& C);

    /* getCameraLoc returns the current camera location in 3-space*/
    Vec3 getCameraLoc() const;
    Vec3 getLookAtLoc() const;

private:
    Mat4 projection;

    double cameraRadius;
    double cameraPhi;
    double cameraTheta;
    Vec3 cameraLookAtLoc;

    Vec2 last;
    double firstX;
    double firstY;
    double firstTheta;
    double firstPhi;
};


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

class TetrahedronApp : public PanApp
{
    Buffer buffer;
    IndexBuffer indexBuffer;
    Geometry geometry;
    Effect effect;
    Assumption material;
    mutable Assumption camera;
    Shape shape;

    void init();
    void draw() const;
};

using namespace std;


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



class ModelApp : public PanApp
{
public:
    char* filename;

#if __APPLE_CC__
    MacFileSystemBank bank;
#else
    UnixBank bank;
#endif

    mutable Model model;

private:
    void init();
    void draw() const;
};

void MyPanEnvironment::init()
{
    initOpenGL();

    model.bank = &bank;

    bank.initSerializableWithPath(&model, filename);
    g2clog( "%s\n", model.serialize().c_str() );
}

void MyPanEnvironment::draw() const
{
    Assumption* context = model.assumptions["context"];
    (*context)["modelView"] = getModelView();
    (*context)["projection"] = getProjection();

    model.draw();
}

int main(int argc, char** args)
{
    MyPanEnvironment e;

    e.filename = (char*)"box.model";
    if( argc > 1 )
        e.filename = args[1];

    e.mainLoop();

    return 0;
}

