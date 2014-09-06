
#include "spriteenvironment.h"

using namespace std;

class ShipEnvironment : public SpriteEnvironment
{
public:
    ShipEnvironment();

private:
    Sprite ship;
    Actor enterprise;

    set<unsigned char> keys;

    Vec2 velocity;
    Vec2 position;
    double theta;

    double then;

    void init();
    void step(double t);
    void draw() const;

    void keyDown(unsigned char c);
    void keyUp(unsigned char c);

    void button();
};

ShipEnvironment::ShipEnvironment() : theta(0.0), then(0.0) {}

void ShipEnvironment::keyDown(unsigned char c)
{
    keys.insert(c);
}

void ShipEnvironment::keyUp(unsigned char c)
{
    keys.erase(c);
}

void ShipEnvironment::init()
{
    bank.initTextureWithPath(&ship, "ship.png");
    ship.numberOfRows = 2;
    ship.numberOfColumns = 32;
    ship.center = true;
    ship.flipRows = true;

    enterprise.sprite = &ship;

    enterprise.position.set(100, 100);
}

void ShipEnvironment::step(double t)
{
    double now = t;

    if( now - then < 1.0 / 30.0 && then != 0.0 )
        return;

    if( keys.find('a') != keys.end() )
        theta += M_PI / 16.0;

    if( keys.find('d') != keys.end() )
        theta -= M_PI / 16.0;

    bool thrust = false;
    if( keys.find('s') != keys.end() ||
        keys.find('w') != keys.end() )
    {
        thrust = true;
        velocity += Vec2(cos(theta), sin(theta));
    }

    enterprise.position += velocity;
    enterprise.frame = (((int)(32.0 * (theta / (2.0 * M_PI)) + 0.5)) % 32 + 32) % 32;

    if( thrust )
        enterprise.frame += 32;

    then = now;
}

void ShipEnvironment::draw() const
{
    enterprise.draw();
}

int main(int argc, char** args)
{
    ShipEnvironment e;
    e.animate = true;
    e.mainLoop();

    return 0;
}

