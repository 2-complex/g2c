
#include "sprites.h"
#include "app.h"
#include "launch.h"
#include "transforms.h"


using namespace std;
using namespace cello;

class AtlasApp : public App
{
public:
    AtlasApp();

private:
    Atlas treeAtlas;
    Actor sherman;
    Bank* bank;
    RendererGL2 renderer;

    void init();
    void resize(int width, int height);
    void setBank(Bank* bank);
    void draw() const;

    void keyDown(unsigned char c);
};

AtlasApp::AtlasApp()
{
}

void AtlasApp::keyDown(unsigned char c)
{
    printf("c = %c\n", c);

    switch( c )
    {
        case '+':
        case '=':
        case '.':
        case '>':
            sherman.frame++;
        break;

        case '-':
        case '_':
        case ',':
        case '<':
            sherman.frame--;
        break;
    }
}

void AtlasApp::setBank(Bank* bank)
{
    this->bank = bank;
}

void AtlasApp::resize(int width, int height)
{
    renderer.projection = orthographic(0,width,0,height,-1,1);
}

void AtlasApp::init()
{
    bank->initTextureWithPath(&treeAtlas, "assets/treeatlas.png");

    treeAtlas.center = true;

    treeAtlas.rectangles.push_back( Rectangle(0, 0, 474, 1014) );
    treeAtlas.rectangles.push_back( Rectangle(476, 3, 780, 279) );
    treeAtlas.rectangles.push_back( Rectangle(476, 285, 762, 511) );
    treeAtlas.rectangles.push_back( Rectangle(488, 519, 768, 739) );
    treeAtlas.rectangles.push_back( Rectangle(482, 756, 915, 1006) );
    treeAtlas.rectangles.push_back( Rectangle(3, 1029, 498, 1594) );
    treeAtlas.rectangles.push_back( Rectangle(497, 1032, 933, 1384) );
    treeAtlas.rectangles.push_back( Rectangle(947, 867, 1368, 1159) );
    treeAtlas.rectangles.push_back( Rectangle(1028, 606, 1584, 856) );
    treeAtlas.rectangles.push_back( Rectangle(779, 354, 1020, 751) );
    treeAtlas.rectangles.push_back( Rectangle(797, 3, 1020, 345) );
    treeAtlas.rectangles.push_back( Rectangle(1025, 240, 1281, 559) );
    treeAtlas.rectangles.push_back( Rectangle(1022, 6, 1284, 229) );
    treeAtlas.rectangles.push_back( Rectangle(1292, 243, 1689, 595) );
    treeAtlas.rectangles.push_back( Rectangle(1298, 9, 1731, 235) );
    treeAtlas.rectangles.push_back( Rectangle(1757, 12, 2013, 505) );
    treeAtlas.rectangles.push_back( Rectangle(1688, 507, 2037, 817) );
    treeAtlas.rectangles.push_back( Rectangle(1388, 867, 1617, 1162) );

    sherman.sampler = &treeAtlas;

    sherman.position.set(300, 300);

    renderer.init();
    Mesh::renderer = &renderer;
}

void AtlasApp::draw() const
{
    sherman.draw();
}

int main(int argc, char** args)
{
    AtlasApp app;
    launch(&app);
    
    return 0;
}

