
#include "panenvironment.h"
#include "graphics.h"

class TetrahedronEnvironment : public PanEnvironment
{
    Buffer* buffer;
    IndexBuffer* indexBuffer;
    Geometry geometry;
    Effect effect;
    Assumption material;
    mutable Assumption camera;
    Shape shape;

    void init();
    void draw() const;  
};

void TetrahedronEnvironment::init()
{
    glewInit();

    float vertexArray[] =
    {
        0.0, 1.0, 0.0,
        -0.5, 0.86603, 0.0,
        -0.5, -0.86603, 0.0,
        0.0, 0.0, 2.0
    };

    int indexArray[] =
    {
        0,2,1,
        0,1,3,
        1,2,3,
        2,0,3
    };

    buffer = new Buffer(vertexArray, 12);
    indexBuffer = new IndexBuffer(indexArray, 12);

    geometry["position"] = Field(buffer, 3, 3, 0);
    geometry.indices = indexBuffer;

    effect.vertexCode = 
        "\n\n"
        "uniform mat4 modelView;\n"
        "uniform mat4 projection;\n"
        "\n"
        "attribute vec3 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * modelView * vec4(position, 1.0);\n"
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

void TetrahedronEnvironment::draw() const
{
    camera["modelView"] = getModelView();
    camera["projection"] = getProjection();

    shape.draw();
}

int main()
{
    TetrahedronEnvironment e;
    e.mainLoop();

    return 0;
}


