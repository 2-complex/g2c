

#include "bugworldapp.h"

#include "macbank.h"
#include "openalplayer.h"

#include "appenvironment.h"


using namespace cello;

int main(int argc, char** args)
{
    AppEnvironment environment;
    BugWorldApp app;

    environment.initWindow("World", 800, 600);

    MacFileSystemBank* bank = new MacFileSystemBank;
    OpenALPlayer* player = new OpenALPlayer;

    app.setBank(bank);
    app.setAudioPlayer(player);

    environment.app = &app;

    environment.mainLoop();

    delete bank;
    delete player;

    return 0;
}

