#ifndef _OPENAL_PLAYER_
#define _OPENAL_PLAYER_

#include "audioplayer.h"

#if __APPLE_CC__
#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <map>

namespace cello
{

class OpenALPlayer : public AudioPlayer
{
public:

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

private:

    struct ContextInfo {
        ALCdevice* device;
        ALCcontext* context;
    };

    std::map<int, ContextInfo> contextInfo;

    static int alcDeviceRefCounter;
    static ALCdevice* alcDevice;

    int getOpenALFormat(int numChannels, int bytesPerSample);
};


}

#endif
