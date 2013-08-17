
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

#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <map>

namespace g2c {

class Player {
public:
	int createContext();
	void destroyContext(int index);
	
	int createSource();
	void destroySource(int index);
	
	int createSound();
	void destroySound(int index);
	
	void makeContextCurrent(int index);
	
	bool isSourcePlaying(int index);
	
	void stopSource(int index);
	
	void loadSound(int index, int sampleRate, int numSamples, int numChannels,
		int bytesPerSample, uint8_t* data);
	
	void playSound(int soundIndex, int sourceIndex, bool loop, double gain);
	
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
