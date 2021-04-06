#include "app.h"

namespace cello
{

App::App()
{
}

App::~App()
{
}

void App::setBank(Bank*)
{
}

void App::setAudioPlayer(AudioPlayer*)
{
}

void App::init()
{
}

void App::step(double /*t*/)
{
}

void App::draw() const
{
}

void App::destroy()
{
}

void App::keyboard(unsigned char /*inkey*/)
{
}

void App::resize(int /*width*/, int /*height*/)
{
    cellolog("WARNING: An app with default resize() function will not respond to the reshaping of the screen.\n");
}

void App::special(int /*inkey*/)
{
}

void App::keyDown(unsigned char /*inkey*/)
{
}

void App::keyUp(unsigned char /*inkey*/)
{
}

bool App::mouseDown(const Vec2& /*C*/)
{
    return false;
}

void App::mouseDragged(const Vec2& /*C*/)
{
}

void App::mouseUp(const Vec2& /*C*/)
{
}

bool App::touchDown(unsigned int index, const Vec2& C)
{
    return false;
}

void App::touchDragged(unsigned int index, const Vec2& C)
{
}

void App::touchUp(unsigned int index, const Vec2& C)
{
}

void App::controllerKeyDown(int deviceId, char c)
{
}

void App::controllerKeyUp(int deviceId, char c)
{
}

void App::controllerMotion(int deviceId, int inputId, double x, double y)
{
}

void App::controllerTriggerMotion(int deviceId, int inputId, double x)
{
}

bool App::getNeedsKeyboard()
{
    return needsKeyboard;
}

void App::showKeyboard()
{
    needsKeyboard = true;
}

void App::hideKeyboard()
{
    needsKeyboard = false;
}

void App::finishTyping()
{
    needsKeyboard = false;
}

} // end namespace

