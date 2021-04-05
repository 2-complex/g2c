#include "emscriptentrampoline.h"

#include <emscripten.h>
#include <SDL.h>
#include <SDL_opengles2.h>

#include <functional>

#include "triangleapp.h"

int main()
{
    TriangleApp app;

    EmscriptenTrampoline trampoline(&app);
    trampoline.initWindow("My Favorite Window", 800, 600);
    trampoline.mainLoop();

    return EXIT_SUCCESS;
}
