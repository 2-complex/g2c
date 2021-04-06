
/*
  cello Copyright (C) 2012 2-Complex

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

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
