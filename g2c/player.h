
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

#ifndef _PLAYER_
#define _PLAYER_

#include <stdint.h>

namespace g2c {

class Player {
public:
	virtual ~Player();
	
	virtual int createContext() = 0;
	virtual void destroyContext(int index) = 0;
	
	virtual int createSource() = 0;
	virtual void destroySource(int index) = 0;
	
	virtual int createSound() = 0;
	virtual void destroySound(int index) = 0;
	
	virtual void makeContextCurrent(int index) = 0;
	
	virtual bool isSourcePlaying(int index) = 0;
	
	virtual void stopSource(int index) = 0;
	
	virtual void loadSound(int index, int sampleRate, int numSamples, int numChannels,
		int bytesPerSample, const uint8_t* data) = 0;
	
	virtual void playSound(int soundIndex, int sourceIndex, bool loop, double gain) = 0;
};

}

#endif
