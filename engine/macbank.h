#ifndef _MAC_BANK_
#define _MAC_BANK_

#include "bank.h"

#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <QuartzCore/QuartzCore.h>

#include <queue>

namespace cello
{

/*! An partial implementation of Bank for Mac OSX.  Contains much of the functionality for loading
    resources from files in iOS as well.  Use the subclass IOSResourceBank to load resources
    in an iOS app.  Use the subclass MacFileSystemBank to load resources from files in a local
    directory strucutre on a Mac.*/
class MacBank : public Bank
{
public:
    MacBank() {}
    virtual ~MacBank() {}
    
    std::string base_path;
    
    virtual void initDataWithPath(Data* data, const char* path);
    
    virtual void initPersistentSerializableWithKey(Serializable* s, const char* key);
    virtual void writePersistentSerializableWithKey(const Serializable* s, const char* key);
    
    virtual void initSerializableWithPath(Serializable* s, const char* path);
    virtual void writeSerializableToPath(const Serializable* s, const char* path);
    virtual void initTextureWithPath(Texture2D* texture, const char* path);
    
    virtual void initSoundWithPath(Sound* sound, const char* path);

    void* getOpenALAudioData(CFURLRef inFileURL,
                     ALsizei* outDataSize,
                     ALenum* outDataFormat,
                     ALsizei* outSampleRate) const;
    
protected:
    std::string directory;
    
    void initTextureWithCGImage(Texture2D* texture, CGImageRef image);
    void initBitmapWithCGImage(Bitmap* bitmap, CGImageRef image);
};

/*! An full implementation of Bank for loading resources from files in the file system on a Mac.*/
class MacFileSystemBank : public MacBank
{
public:
    MacFileSystemBank() {}
    virtual ~MacFileSystemBank() {}
    virtual void initBitmapWithPath(Bitmap* bitmap, const char* path);
};

} // end namespace


#endif

