
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

#include "wave.h"
#include "serializable.h"
#include "player.h"

namespace g2c {

class Context;

class Context {
friend class Sound;
friend class Source;
public:
    explicit Context(Player* player);
    virtual ~Context();
    
    void makeCurrent();
	
private:
	int index;
	Player* player;
	
	static Context* currentContext;
};

class Source {
friend class Sound;
public:
    Source();
    virtual ~Source();
    
    bool isPlaying() const;
	
private:
	int index;
	Player* player;
};

class Sound : public Serializable {
public:
    Sound();
    virtual ~Sound();
    
    std::string file;
    
    mutable Source* source;
    
    bool loop;
    
    void initWithWave(const Wave& wave);
    
    void play(double gain = 1.0) const;
    void stop() const;
    
    virtual std::string serializeElements(std::string indent = "") const;
    virtual void handleChild(const parse::Node* n);
    
    // Temprary fix for MacBank which has code in it which appeals directly to OpenAL.
    // Sheesh.
    int getIndex() {return index;}
    
private:
	int index;
	Player* player;
	
	void load(int sampleRate, int numSamples, int numChannels,
		int bytesPerSample, uint8_t* data);
};

} // end namespace

#endif
