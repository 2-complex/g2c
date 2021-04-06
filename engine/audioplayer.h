#ifndef _AUDIO_PLAYER_
#define _AUDIO_PLAYER_

#include <stdint.h>

namespace cello
{

class AudioPlayer
{
public:
    virtual ~AudioPlayer();

    virtual int createContext();
    virtual void destroyContext(int index);

    virtual int createSource();
    virtual void destroySource(int index);

    virtual int createSound();
    virtual void destroySound(int index);

    virtual void makeContextCurrent(int index);

    virtual bool isSourcePlaying(int index);

    virtual void stopSource(int index);

    virtual void loadSound(int index, int sampleRate, int numSamples, int numChannels,
        int bytesPerSample, const uint8_t* data);

    virtual void playSound(int soundIndex, int sourceIndex, bool loop, double gain);
};

}

#endif
