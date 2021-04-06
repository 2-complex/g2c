#ifndef _LISTENER_
#define _LISTENER_

#include "lin/lin.h"

#include <set>

namespace cello
{


class Listener;

class Listener {
    
private:
    std::set<unsigned char> keys;

public:
    Listener();
    virtual ~Listener();

    /*! keyboard gets called when a key should type a letter, i.e. once at the
        same time as keyDown and then, after a pause, repeatedly according to
        the keyboard repeat rate.*/
    virtual void keyboard(unsigned char inkey);

    /*! keyDown gets called when a key is first pressed.
        Override to handle key events.*/
    virtual void keyDown(unsigned char inkey);

    /*! keyUp gets called when a key is released.
        Override to handle key events.*/
    virtual void keyUp(unsigned char inkey);

    /*! mouseDown() gets called when the mouse button is first depressed.
        Override mouseDown to implement new mouse click behavior.  Return true
        to stop Environment from interpreting the mouse event and panning the
        camera.*/
    virtual bool mouseDown(const Vec2& C);

    /*! mouseDragged() gets called when the mouse is dragged.*/
    virtual void mouseDragged(const Vec2& C);

    /*! gets called when the mouse is released.*/
    virtual void mouseUp(const Vec2& C);

    bool listening;

    /*! The base-class behavior is to appeal to the delegate if it's non-null
        for each virtual function.*/
    Listener* delegate;
};

} // end namespace


#endif
