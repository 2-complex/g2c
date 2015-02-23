
#include "app.h"
#include "graphics.h"
#include "launch.h"

#include <string>

using namespace g2c;
using namespace std;


class ModelApp : public App
{
public:
    ModelApp();
    virtual ~ModelApp();

    Bank* bank;
    Model* model;
    std::string filename;

    void setCenter(const Vec3& V);
    void setBank(Bank* inBank);

    void init();
    void draw() const;

    void resize(int width, int height);
    bool mouseDown(const Vec2& C);
    void mouseDragged(const Vec2& C);
    void mouseUp(const Vec2& C);

protected:
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


ModelApp::ModelApp() : bank(NULL)
{
    cameraRadius = 20;
    cameraPhi = 0.5;
    cameraTheta = -0.25;

    firstX = 0;
    firstY = 0;
    firstTheta = 0;
    firstPhi = 0;

    model = new Model;
}

ModelApp::~ModelApp()
{
    delete model;
    delete model->bank;
}

void ModelApp::resize(int width, int height)
{
    projection = perspective(
        20.0,
        1.0 * width/height,
        0.1 * cameraRadius,
        1000 * cameraRadius );
}

void ModelApp::init()
{
    model->bank->initSerializableWithPath(model, filename.c_str());
    this->filename = filename;
}

Vec3 ModelApp::getCameraLoc() const
{
    return cameraLookAtLoc+
        cameraRadius*Vec3(cos(cameraTheta)*cos(cameraPhi),
                              sin(cameraTheta)*cos(cameraPhi),
                              sin(cameraPhi));
}

bool ModelApp::mouseDown(const Vec2& C)
{
    last = C;
    return true;
}

void ModelApp::mouseDragged(const Vec2& C)
{
    Vec2 delta = -0.01*(C - last);
    cameraTheta += delta.x;
    cameraPhi += delta.y;

    if(cameraPhi > 0.5*M_PI) cameraPhi = 0.5*M_PI;
    if(cameraPhi < -0.5*M_PI) cameraPhi = -0.5*M_PI;

    last = C;
}

void ModelApp::mouseUp(const Vec2& C)
{
    mouseDragged(C);
}

void ModelApp::setBank(Bank* inBank)
{
    bank = inBank;
    model->bank = bank;
}

void ModelApp::setCenter(const Vec3& V)
{
    cameraLookAtLoc = V;
}

Vec3 ModelApp::getLookAtLoc() const
{
    return cameraLookAtLoc;
}

void ModelApp::draw() const
{
    if( model->assumptions.find("context") == model->assumptions.end() )
    {
        printf( "No context found in model.\n" );
        printf( "Model must have assumption called 'context'.\n" );
        exit(0);
    }

    Assumption& context = *(model->assumptions["context"]);

    Mat4 world;
    Mat4 view = lookAt(getCameraLoc(), getLookAtLoc(), Vec3(0,0,1));

    context["model"] = world;
    context["viewProjection"] = projection * view;

    context["modelView"] = view * world;
    context["projection"] = projection;
    context["cameraPosition"] = getCameraLoc();

    model->draw();
}

int main(int argc, char** args)
{
    ModelApp app;

    app.filename = "box.model";
    if( argc > 1 )
        app.filename = args[1];

    launch(&app);

    return 0;
}

