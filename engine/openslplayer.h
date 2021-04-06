#ifndef _OPENSL_PLAYER_
#define _OPENSL_PLAYER_

#include "audioplayer.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include <map>

namespace cello
{


class OpenSLPlayer : public AudioPlayer {
friend void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *ptr);
public:
    OpenSLPlayer();

    virtual int createContext();
    virtual void destroyContext(int index);

    virtual int createSource();
    virtual void destroySource(int index);

    virtual int createSound();
    virtual void destroySound(int index);

    virtual void makeContextCurrent(int index);

    virtual void loadSound(int index, int sampleRate, int numSamples, int numChannels,
        int bytesPerSample, const uint8_t* data);

    virtual void playSound(int soundIndex, int sourceIndex, bool loop, double gain);

    virtual bool isSourcePlaying(int index);

    virtual void stopSource(int index);

private:
    struct ContextInfo {
        SLObjectItf engineObject;
        SLEngineItf engine;
        SLObjectItf outputMixObject;
        SLEnvironmentalReverbItf outputMixEnvironmentalReverb; // unused for now.
    };

    class Audio {
    public:
        Audio();
        virtual ~Audio();

        uint8_t* buffer;
        unsigned int size;
    };

    class SourceInfo {
    public:
        SourceInfo();
        virtual ~SourceInfo();

        bool loop;
        Audio* audio;

        // buffer queue player interfaces
        SLObjectItf bqPlayerObject;
        SLPlayItf bqPlayerPlay;
        SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
        SLEffectSendItf bqPlayerEffectSend;
        SLMuteSoloItf bqPlayerMuteSolo;
        SLVolumeItf bqPlayerVolume;

        void realize();
    };


    int contextIndex;
    std::map<int, ContextInfo> contextInfos;
    std::map<int, SourceInfo*> sourceInfos;
    std::map<int, Audio*> audios;
};


}

#endif