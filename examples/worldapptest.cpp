
#include "gluttrampoline.h"
#include "worldapp.h"

#if __APPLE_CC__
#include "macbank.h"
#else
#include "unixbank.h"
#endif

#include "openalplayer.h"

int main(int argc, char** args)
{
    GlutTrampoline trampoline;
    WorldApp app;

#if __APPLE_CC__
    MacFileSystemBank bank;
#else
    UnixBank bank;
#endif

    OpenALPlayer player;

    trampoline.initWindow("World", 800, 600);

    app.setBank(&bank);
    app.setAudioPlayer(&player);

    trampoline.app = &app;
    trampoline.mainLoop();

    return 0;
}

