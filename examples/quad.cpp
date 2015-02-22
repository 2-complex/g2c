

#include "graphics.h"
#include "launch.h"

using namespace g2c;

class QuadApp : public App
{
    Bank* bank;

    Buffer* buffer;
    IndexBuffer* indexBuffer;
    Geometry geometry;
    Effect effect;
    Assumption material;
    mutable Assumption camera;
    Texture2D texture;
    Shape shape;

    void init();
    void draw() const;
    void setBank(Bank* bank);
};

void QuadApp::init()
{
    float vertexArray[] =
    {
        -1, -1, 0,
         1, -1, 0,
         1,  1, 0,
        -1,  1, 0,
    };

    int indexArray[] =
    {
        0, 2, 1,
        0, 2, 3
    };

    buffer = new Buffer(vertexArray, 12);
    indexBuffer = new IndexBuffer(indexArray, 6);

    geometry["position"] = Field(buffer, 3, 3, 0);
    geometry.indices = indexBuffer;

    bank->initTextureWithPath(&texture, "tiles.png");

    effect.vertexCode =
        "\n"
        "varying vec3 v_position;\n"
        "\n"
        "attribute vec3 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    v_position = position.xyz;\n"
        "    gl_Position = vec4(position, 1.0);\n"
        "}\n";

    effect.fragmentCode =
        "\n"
        "uniform sampler2D sampler;\n"
        "varying vec3 v_position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = texture2D(sampler, v_position.xy);\n"
        "}\n";

    effect.compile();

    material.effect = &effect;

    shape.geometry = &geometry;
    shape.assumptions.push_back(&material);
}

void QuadApp::setBank(Bank* bank)
{
    this->bank = bank;
}

void QuadApp::draw() const
{
    shape.draw();
}

LAUNCH(QuadApp)

