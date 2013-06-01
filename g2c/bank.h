
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


#ifndef _BANK_
#define _BANK_

#include "serializable.h"
#if !defined(STUB_SOUND)
#include "sound.h"
#endif
#include "texture.h"

#include <queue>

namespace g2c {




class Bank {
public:
    Bank() {}
    virtual ~Bank() {}
    
    virtual void initPersistentSerializableWithKey(Serializable* s, const char* key) = 0;
    virtual void writePersistentSerializableWithKey(const Serializable* s, const char* key) = 0;
    
    virtual void initSerializableWithPath(Serializable* s, const char* path) = 0;
    virtual void writeSerializableToPath(const Serializable* s, const char* path) = 0;
#if !defined(STUB_SOUND)
    virtual void initSoundWithPath(Sound* sound, const char* path) = 0;
#endif
    virtual void initTextureWithPath(Texture2D* texture, const char* path) = 0;
    virtual void initBitmapWithPath(Bitmap* bitmap, const char* path) = 0;
};

class AsynchronousBank : public Bank {
public:
    AsynchronousBank() : bank(NULL), biggestSize(1) {}
    virtual ~AsynchronousBank() {}
    
    Bank* bank;
    
    struct LoadInstruction {
        LoadInstruction(Serializable* resource, const std::string& path) :
            resource(resource), path(path) {}
        Serializable* resource;
        std::string path;
    };
    
    virtual void initPersistentSerializableWithKey(Serializable* s, const char* key);
    virtual void writePersistentSerializableWithKey(const Serializable* s, const char* key);
    
    virtual void initSerializableWithPath(Serializable* s, const char* path);
    virtual void writeSerializableToPath(const Serializable* s, const char* path);

#if !defined(STUB_SOUND)
    virtual void initSoundWithPath(Sound* sound, const char* path);
#endif
    virtual void initTextureWithPath(Texture2D* texture, const char* path);
    
    bool step();
    int percent() const;

private:
    mutable int biggestSize;
    std::queue<LoadInstruction> q;
};

} // end namespace


#endif
