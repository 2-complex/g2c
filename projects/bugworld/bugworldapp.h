
#include "app.h"
#include "bank.h"
#include "sprites.h"

class BugWorldApp : public cello::App
{
public:
    virtual void setBank(cello::Bank* inBank);

    virtual void init();
    virtual void reshape(int width, int height);
    virtual void draw() const;
    virtual void destroy();

private:
    cello::Bank* bank;
    cello::World* world;
    cello::Renderer* renderer;
};

