
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



#include "sound.h"
#include "util.h"

#include "log.h"

int Context::alcDeviceRefCounter = 0;
ALCdevice* Context::alcDevice = NULL;


using namespace std;

Context::Context() : device(NULL), context(NULL)
{
	if(!Context::alcDevice)
		Context::alcDevice = alcOpenDevice(NULL);
	
	device = Context::alcDevice;
	alcDeviceRefCounter++;
	
	context = alcCreateContext(device, 0);
	
	ALenum  error = AL_NO_ERROR;
	if((error = alGetError()) != AL_NO_ERROR)
	{
		g2cerror("OpenAL error in context initialization: 0x%x\n", error);
		exit(0);
	}
}

Context::~Context()
{
	alcSuspendContext(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	
	alcDeviceRefCounter--;
	if(alcDeviceRefCounter==0)
		alcCloseDevice(device);
}

void Context::makeCurrent()
{
	alcMakeContextCurrent(context);
}

Source::Source() : source(0)
{
	alGenSources(1, &source);
}

Source::~Source()
{
	
}

bool Source::isPlaying() const
{
	ALint state = 0;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

Sound::Sound() : buffer(0), source(NULL), loop(false)
{
	type = "Sound";
	alGenBuffers(1, &buffer);
}

Sound::~Sound()
{
	
}

void Sound::play() const
{
	if(source)
	{
		alSourcei(source->source, AL_BUFFER, buffer);
		alSourcei(source->source, AL_LOOPING, loop);
		alSourcePlay(source->source);
	}
}

void Sound::useSource(Source* inSource) const
{
	source = inSource;
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