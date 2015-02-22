

#include "graphics.h"
#include "app.h"

#include <iostream>


using namespace std;
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


void TetrahedronApp::init()
{
    float vertexArray[] =
    {
         1.0,  0.0,     -0.25 * 1.41421,
        -0.5,  0.86603, -0.25 * 1.41421,
        -0.5, -0.86603, -0.25 * 1.41421,
         0.0,  0.0,      0.75 * 1.41421
    };

    int indexArray[] =
    {
        0,2,1,
        0,1,3,
        1,2,3,
        2,0,3
    };

    buffer.set(vertexArray, sizeof(vertexArray) / sizeof(float));
    indexBuffer.set(indexArray, sizeof(indexArray) / sizeof(int));

    geometry["position"] = Field(&buffer, 3, 3, 0);
    geometry.indices = &indexBuffer;

    effect.vertexCode =
        "\n"
        "uniform mat4 modelView;\n"
        "uniform mat4 projection;\n"
        "\n"
        "attribute vec3 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * modelView\n"
        "       * vec4(position, 1.0);\n"
        "}\n";

    effect.fragmentCode =
        "\n"
        "uniform vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = color;\n"
        "}\n";

    effect.compile();

    material.effect = &effect;

    shape.geometry = &geometry;
    shape.assumptions.push_back(&camera);
    shape.assumptions.push_back(&material);

    material["color"] = Vec4(1.0, 0.0, 0.0, 1.0);
}

void TetrahedronApp::draw() const
{
    camera["modelView"] = lookAt(
        getCameraLoc(),
        getLookAtLoc(),
        Vec3(0, 0, 1));

    camera["projection"] = getProjection();

    shape.draw();
}





using namespace std;

GLuint programObject;
SDL_Surface* screen;
App* app = new TetrahedronApp;

GLfloat vVertices[] = {
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
};
GLint uniformOriginX, uniformOriginY, uniformZoom;


void initSDL(int width, int height)
{
    static bool initializedAlready = false;

    if( ! initializedAlready )
    {
        //initialise SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0) 
        {
            screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL);
            if (screen == NULL)
            {
                cerr << "Could not set video mode: " << SDL_GetError() << endl;
            }
        }
        else 
        {
            cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
        }
        initializedAlready = true;
    }
}

extern "C" int initGL(int width, int height)
{
    initSDL(width, height);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glViewport(0, 0, width, height);

    return 1;
}

extern "C" void mouseDown(double x, double y)
{
    printf( "mouseDown(%f, %f)\n", x, y );
}

extern "C" void mouseDragged(double x, double y)
{
    printf( "mouseDragged(%f, %f)\n", x, y );
}

extern "C" void mouseUp(double x, double y)
{
    printf( "mouseUp(%f, %f)\n", x, y );
}

extern "C" void init()
{
    printf("init\n");
    app->init();
}

extern "C" void resize(int width, int height)
{
    printf("resize %d %d\n", width, height);
    initGL(width, height);

    app->resize(width, height);
}

extern "C" void step(double t)
{
    printf("step %f\n", t);

    app->step(t);
}

extern "C" void draw()
{
    printf("draw\n");
    float a[] = {0.0f,0.0f,1.0f};

    app->draw();

    SDL_GL_SwapBuffers();
}

