#include "app.h"
#include "graphics.h"
#include "launch.h"

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

void TriangleApp::init()
{
    effect.vertexCode =
        "attribute vec4 position;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(position.xyz, 1.0);\n"
        "}\n";

    effect.fragmentCode =
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(0.7, 0.1, 0.5, 1.0);\n"
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

int main()
{
    TriangleApp app;
    launch(&app);
    return 0;
}
