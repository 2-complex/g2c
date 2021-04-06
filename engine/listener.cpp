#include "listener.h"
#include "opengl.h"

namespace cello
{

Listener* gListener = NULL;

Listener::Listener() : listening(true), delegate(NULL) {}
Listener::~Listener() {}

void Listener::keyboard(unsigned char inkey)
{
    if( delegate )
        delegate->keyboard(inkey);
}

void Listener::keyDown(unsigned char inkey)
{
    if( delegate )
        delegate->keyDown(inkey);
}

void Listener::keyUp(unsigned char inkey)
{
    if( delegate )
        delegate->keyUp(inkey);
}

bool Listener::mouseDown(const Vec2& C)
{
    if( delegate )
        return delegate->mouseDown(C);
    return false;
}

void Listener::mouseDragged(const Vec2& C)
{
    if( delegate )
        delegate->mouseDragged(C);
}

void Listener::mouseUp(const Vec2& C)
{
    if( delegate )
        delegate->mouseUp(C);
}

} // end namespace

