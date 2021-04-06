#include "sound.h"
#include "util.h"

#include "log.h"

#include <stdlib.h>

using namespace std;

namespace cello
{

Context* Context::currentContext = NULL;

Context::Context(AudioPlayer* player) : index(0), player(player)
{
    index = player->createContext();
}

Context::~Context()
{
    player->destroyContext(index);
    if( Context::currentContext == this )
        Context::currentContext = NULL;
}

void Context::makeCurrent()
{
    if( index <= 0 )
    {
        celloerror("Attempt to make context current which was not initialized properly.\n");
        exit(0);
    }

    currentContext = this;
    player->makeContextCurrent(index);
}

Source::Source() : index(0)
{
    if( !Context::currentContext )
    {
        celloerror("Attempt to create Source with no current context.\n");
        exit(0);
    }

    player = Context::currentContext->player;

    if( !player )
    {
        celloerror("Attempt to make Source with no player.\n");
        exit(0);
    }

    index = player->createSource();
}

Source::~Source()
{
    if( index <= 0 )
    {
        celloerror("Attempt to destroy Source which was not initialized properly.\n");
        exit(0);
    }

    player->destroySource(index);
}

bool Source::isPlaying() const
{
    return player->isSourcePlaying(index);
}

Sound::Sound() : index(0), source(NULL), loop(false)
{
    type = "Sound";

    if( !Context::currentContext )
    {
        celloerror("Attempt to create Sound with no current context.\n");
        exit(0);
    }

    player = Context::currentContext->player;

    if( !player )
    {
        celloerror("Attempt to create Sound with no player set.\n");
        exit(0);
    }

    index = player->createSound();

    if( index <= 0 )
    {
        celloerror("Player failed to create sound.\n");
        exit(0);
    }
}

Sound::~Sound()
{
    if( index <= 0 )
    {
        celloerror("Attempt to destroy Sound which was not initialized properly.\n");
        exit(0);
    }

    player->destroySound(index);
}

void Sound::play(double gain) const
{
    if( !source )
    {
        celloerror("Attempt to play Sound with no source.\n");
        exit(0);
    }

    player->playSound(index, source->index, loop, gain);
}

void Sound::stop() const
{
    if( !source )
    {
        celloerror("Attempt to stop Sound with no source.\n");
        exit(0);
    }

    player->stopSource(source->index);
}

string Sound::serializeElements(std::string indent) const
{
    string r = Serializable::serializeElements(indent);
    r += TAB + indent + "'file' : " + string("'") + file + "',\n";
    if(loop)
        r += TAB + indent + "'loop' : " + intToString(loop) + ",\n";
    return r;
}

void Sound::handleChild(const parse::Node* n)
{
    string n_name = n->data.s;
    parse::Node* value = n->data.value;
    if(n_name == "file")
        file = value->data.s;

    if(n_name == "name")
        name = value->data.s;

    if(n_name == "file")
        file = value->data.s;

    if(n_name == "loop")
        loop = value->data.i;
}

void Sound::load(int sampleRate, int numSamples, int numChannels,
    int bytesPerSample, const uint8_t* data)
{
    if( !player )
    {
        celloerror("Attempt to load sound with no player.");
        exit(0);
    }

    player->loadSound(index, sampleRate, numSamples, numChannels,
        bytesPerSample, data);
}

void Sound::initWithWave(const Wave& wave)
{
    load(wave.sampleRate, wave.numSamples, wave.numChannels,
        wave.bytesPerSample, wave.data);
}


} // end namespace

