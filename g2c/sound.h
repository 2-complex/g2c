
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



#ifndef _SOUND_
#define _SOUND_

#include "serializable.h"

#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

class Context {
public:
	Context();
	virtual ~Context();
	
	void makeCurrent();
	
	ALCdevice* device;
	ALCcontext* context;
	
	static int alcDeviceRefCounter;
	static ALCdevice* alcDevice;
};

class Source {
public:
	Source();
	virtual ~Source();
	
	ALuint source;
	
	bool isPlaying() const;
};

class Sound : public Serializable {
public:
	Sound();
	virtual ~Sound();
	
	std::string file;
	
	ALuint buffer;
	bool loop;
	
	void useSource(Source* inSource) const;
	void play() const;
	
	virtual std::string serializeElements(std::string indent = "") const;
	virtual void handleChild(const parse::Node* n);
	
private:
	mutable Source* source;
	void* getOpenALAudioData(CFURLRef inFileURL,
							 ALsizei* outDataSize,
							 ALenum* outDataFormat,
							 ALsizei* outSampleRate);
};

#endif