#ifndef _ANIMATIONS_
#define _ANIMATIONS_

#include "sprites.h"

namespace cello
{

class MoveVec2 : public Animation {
public:
    MoveVec2(double instart, double induration, Vec2* inv, const Vec2& indst);
    
    Vec2* v;
    Vec2 src;
    Vec2 dst;
    virtual void begin();
    virtual void step(double t);
    virtual void end();
};

class MoveMatrix : public Animation {
public:
    MoveMatrix(double instart, double induration, Mat4* inm, const Mat4& indst);
    
    Mat4* m;
    Mat4 src;
    Mat4 dst;
    virtual void begin();
    virtual void step(double t);
    virtual void end();
};

class WalkInt : public Animation {
public:
    WalkInt(double instart, double induration, int* ina, int infirst, int inlast);
    
    int* a;
    int first;
    int last;
    virtual void begin();
    virtual void step(double t);
    virtual void end();
};

class StopEvents : public Animation {
public:
    StopEvents(double instart, double induration);
    
    virtual void begin();
    virtual void step(double t);
    virtual void end();
};

class Transition : public StopEvents {
public:
    Transition(double instart, double induration, Node* outgoing, Node* incoming);
    
    Node* outgoing;
    Node* incoming;
    
    virtual void begin();
    virtual void step(double t);
    virtual void end();
};

class FadeIn: public Animation {
public:
    FadeIn(double instart, double induration, Node* node, bool inStopsEvents = true);
    
    Node* node;
    
    virtual void begin();
    virtual void step(double t);
    virtual void end();
};

class FadeOut: public Animation {
public:
    FadeOut(double instart, double induration, Node* node, bool inStopsEvents = true);
    
    Node* node;
    
    virtual void begin();
    virtual void step(double t);
    virtual void end();
};

} // end namespace

#endif
