#ifndef _UNIX_BANK_
#define _UNIX_BANK_

#include "bank.h"

#include <queue>

namespace cello
{

/*! An implementation of Bank for linux/unix-based systems.  Loads uses unix libraries to load
    bitmap data from image files, raw audio from sound files etc, uses standard libaries for
    loading raw data from a file. */
class UnixBank : public Bank {
public:
    UnixBank() {}
    virtual ~UnixBank() {}
    
    std::string base_path;

    virtual void initPersistentSerializableWithKey(Serializable* s, const char* key);
    virtual void writePersistentSerializableWithKey(const Serializable* s, const char* key);
    virtual void initDataWithPath(Data* data, const char* path);
    virtual void initSerializableWithPath(Serializable* s, const char* path);
    virtual void writeSerializableToPath(const Serializable* s, const char* path);
    virtual void initTextureWithPath(Texture2D* texture, const char* path);
    virtual void initBitmapWithPath(Bitmap* bitmap, const char* path);
    virtual void initSoundWithPath(Sound* sound, const char* path);

protected:
    std::string directory;
};

} // end namespace

#endif

