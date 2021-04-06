#ifndef _IOS_RESOURCE_BANK_
#define _IOS_RESOURCE_BANK_

#include "macbank.h"

namespace cello
{

/*! A full implementation of Bank for loading resources from the resources group in an iOS XCode project. */
class IOSResourceBank : public MacBank {
public:
    IOSResourceBank() {}
    virtual ~IOSResourceBank() {}

    virtual void initPersistentSerializableWithKey(Serializable* s, const char* key);
    virtual void writePersistentSerializableWithKey(const Serializable* s, const char* key);

    virtual void initSerializableWithPath(Serializable* world, const char* path);
    virtual void writeSerializableToPath(const Serializable* world, const char* path);

    virtual void initSoundWithPath(Sound* sound, const char* path);
    virtual void initBitmapWithPath(Bitmap* bitmap, const char* path);
};

} // end namespace


#endif
