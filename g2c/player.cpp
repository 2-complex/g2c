
/*
  g2c Copyright (C) 2012 2-Complex

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

#include "player.h"
#include "log.h"

namespace g2c {

Player::~Player() {}

int Player::createContext()
{
    g2clog("Stub Player context created\n");
    return 1;
}

void Player::destroyContext(int index)
{
    g2clog("Stub Player context destroyed\n");
}

int Player::createSource()
{
    g2clog("Stub Player source created\n");
    return 1;
}

void Player::destroySource(int index)
{
    g2clog("Stub Player source destroyed\n");
}

int Player::createSound()
{
    g2clog("Stub Player sound created\n");
    return 1;
}

void Player::destroySound(int index)
{
    g2clog("Stub Player sound destroyed\n");
}

void Player::makeContextCurrent(int index)
{
    g2clog("Stub Player context made current\n");
}

bool Player::isSourcePlaying(int index)
{
    g2clog("Stub Player asked if source is playing\n");
    return true;
}

void Player::stopSource(int index)
{
    g2clog("Stub Player source stopped\n");
}

void Player::loadSound(int index, int sampleRate, int numSamples, int numChannels,
    int bytesPerSample, const uint8_t* data)
{
    g2clog("Stub Player sound data loaded\n");
}

void Player::playSound(int soundIndex, int sourceIndex, bool loop, double gain)
{
    g2clog("Stub Player sound played\n");
}

}