
#include "sound.h"
#include "app.h"
#include "launch.h"

using namespace cello;

class SoundApp : public App
{
    Context* context;
    Sound* sound;
    Source* source;
    Bank* bank;
    AudioPlayer* player;

    void init();
    void destroy();
    void setBank(Bank* bank);
    void setAudioPlayer(AudioPlayer* player);
    bool mouseDown(const Vec2& c);
};

void SoundApp::init()
{
    context = new Context(player);
    context->makeCurrent();

    sound = new Sound;
    source = new Source;

    bank->initSoundWithPath(sound, "assets/harmonica.wav");
    sound->source = source;
}

void SoundApp::destroy()
{
    delete sound;
    delete source;
    delete context;
}

void SoundApp::setBank(Bank* bank)
{
    this->bank = bank;
}

void SoundApp::setAudioPlayer(AudioPlayer* player)
{
    this->player = player;
}

bool SoundApp::mouseDown(const Vec2& c)
{
    sound->play();
    return true;
}

int main()
{
    SoundApp app;
    launch(&app);

    return 0;
}

