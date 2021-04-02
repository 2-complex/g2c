#include <functional>

#include <emscripten.h>
#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

// an example of something we will control from the javascript side
bool background_is_black = true;


// the function called by the javascript code
extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color()
{
    background_is_black = !background_is_black;
}

#include "g2c/graphics.h"
#include "g2c/app.h"
#include "g2c/serializable.h"


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

    int indexArray[] =
    {
        0,2,1
    };

    buffer.set(vertexArray, sizeof(vertexArray) / sizeof(float));
    indexBuffer.set(indexArray, sizeof(indexArray) / sizeof(int));

    field = Field(&buffer, 2, 2, 0);
    geometry["position"] = field;
    geometry.indices = &indexBuffer;


    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void TriangleApp::draw() const
{
    effect.use();
    effect.addAttribute("position");
    effect.bindAttributeToField("position", field);
    glDrawElements(GL_TRIANGLES, geometry.indices->size, GL_UNSIGNED_SHORT, 0);
}


std::function<void()> loop;
void main_loop() { loop(); }

int main()
{
    TriangleApp app;

    SDL_Window *window;
    SDL_CreateWindowAndRenderer(640, 480, 0, &window, nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    app.init();
    double clock = 0.0;

    loop = [&]
    {
        if( background_is_black )
        {
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        }
        else
        {
            glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        }

        glClearColor(1.0f, 0.0, 0.4f, 1.0f);
        glClearDepthf(0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        app.step(clock);
        clock += 0.01;

        app.draw();

        SDL_GL_SwapWindow(window);
    };

    emscripten_set_main_loop(main_loop, 0, true);

    return EXIT_SUCCESS;
}
