#ifndef _WINDOWS_BANK_
#define _WINDOWS_BANK_

#include "bank.h"

namespace cello
{

/*! An implementation of Bank for loading resources from a windows native app.*/
class WindowsBank : public Bank {
public:
    WindowsBank() {}
    virtual ~WindowsBank() {}
    
    std::string base_path;
    
    virtual void initPersistentSerializableWithKey(Serializable* s, const char* key);
    virtual void writePersistentSerializableWithKey(const Serializable* s, const char* key);
    
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
