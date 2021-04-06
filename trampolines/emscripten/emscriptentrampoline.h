#ifndef _EMSCRIPTEN_TRAMPOLINE_
#define _EMSCRIPTEN_TRAMPOLINE_

#include "app.h"
#include "bank.h"

#include <SDL.h>

using namespace cello;

class EmscriptenTrampoline
{
public:
    explicit EmscriptenTrampoline(App* app);
    virtual ~EmscriptenTrampoline();

    void initWindow(const std::string& windowTitle, int width, int height);
    void mainLoop();

private:
    SDL_Window *window;

public:
    App* app;
};

#endif
