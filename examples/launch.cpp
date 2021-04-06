#include "gluttrampoline.h"

void launch(App* app)
{
    GlutTrampoline trampoline(app);
    trampoline.initWindow("World", 800, 600);
    trampoline.mainLoop();
}
