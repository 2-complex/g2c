
#include "appenvironment.h"
#include "worldapp.h"

#if __APPLE_CC__
#include "macbank.h"
#else
#include "unixbank.h"
#endif

#include "openalplayer.h"

int main(int argc, char** args)
{
    AppEnvironment environment;
    WorldApp app;

#if __APPLE_CC__
    MacFileSystemBank bank;
#else
    UnixBank bank;
#endif

    OpenALPlayer player;

    environment.initWindow("World", 800, 600);

    app.setBank(&bank);
    app.setAudioPlayer(&player);

    environment.app = &app;

    environment.mainLoop();

    return 0;
}

