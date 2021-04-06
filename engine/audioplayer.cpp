#include "audioplayer.h"
#include "log.h"

namespace cello
{

AudioPlayer::~AudioPlayer() {}

int AudioPlayer::createContext()
{
    cellolog("Stub AudioPlayer context created\n");
    return 1;
}

void AudioPlayer::destroyContext(int index)
{
    cellolog("Stub AudioPlayer context destroyed\n");
}

int AudioPlayer::createSource()
{
    cellolog("Stub AudioPlayer source created\n");
    return 1;
}

void AudioPlayer::destroySource(int index)
{
    cellolog("Stub AudioPlayer source destroyed\n");
}

int AudioPlayer::createSound()
{
    cellolog("Stub AudioPlayer sound created\n");
    return 1;
}

void AudioPlayer::destroySound(int index)
{
    cellolog("Stub AudioPlayer sound destroyed\n");
}

void AudioPlayer::makeContextCurrent(int index)
{
    cellolog("Stub AudioPlayer context made current\n");
}

bool AudioPlayer::isSourcePlaying(int index)
{
    cellolog("Stub AudioPlayer asked if source is playing\n");
    return true;
}

void AudioPlayer::stopSource(int index)
{
    cellolog("Stub AudioPlayer source stopped\n");
}

void AudioPlayer::loadSound(int index, int sampleRate, int numSamples, int numChannels,
    int bytesPerSample, const uint8_t* data)
{
    cellolog("Stub AudioPlayer sound data loaded\n");
}

void AudioPlayer::playSound(int soundIndex, int sourceIndex, bool loop, double gain)
{
    cellolog("Stub AudioPlayer sound played\n");
}

}
