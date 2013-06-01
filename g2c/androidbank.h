
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



#ifndef _ANDROID_BANK_
#define _ANDROID_BANK_

#include "bank.h"

#include <jni.h>

namespace g2c {




class AndroidBank : public Bank {
public:
	AndroidBank() : env(NULL), loader(NULL) {}
	virtual ~AndroidBank() {}
	
	std::string base_path;
	
	void setEnvAndLoader(JNIEnv* env, jobject loader);
	
	virtual void initPersistentSerializableWithKey(Serializable* s, const char* key);
	virtual void writePersistentSerializableWithKey(const Serializable* s, const char* key);
	
	virtual void initSerializableWithPath(Serializable* s, const char* path);
	virtual void writeSerializableToPath(const Serializable* s, const char* path);
	virtual void initTextureWithPath(Texture2D* texture, const char* path);
	virtual void initBitmapWithPath(Bitmap* bitmap, const char* path);

#if !defined(STUB_SOUND)
	virtual void initSoundWithPath(Sound* sound, const char* path);
	void* getOpenALAudioData(CFURLRef inFileURL,
					 ALsizei* outDataSize,
					 ALenum* outDataFormat,
					 ALsizei* outSampleRate) const;
#endif

protected:
	std::string directory;
	
	JNIEnv* env;
	jobject loader;
};

}

#endif

