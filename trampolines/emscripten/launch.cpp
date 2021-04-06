#include "emscriptentrampoline.h"

void launch(App* app)
{
    EmscriptenTrampoline trampoline(app);
    trampoline.initWindow("World", 800, 600);
    trampoline.mainLoop();
}
