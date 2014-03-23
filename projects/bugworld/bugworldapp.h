
#include "app.h"
#include "bank.h"
#include "sprites.h"

class BugWorldApp : public g2c::App
{
public:
    virtual void setBank(g2c::Bank* inBank);

    virtual void init();
    virtual void reshape(int width, int height);
    virtual void draw() const;
    virtual void destroy();

private:
    g2c::Bank* bank;
    g2c::World* world;
    g2c::Renderer* renderer;
};

