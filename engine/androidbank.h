#ifndef _ANDROID_BANK_
#define _ANDROID_BANK_

#include "bank.h"

#include <jni.h>

namespace cello
{


/*! An implementation of Bank for use in an Android app.  Loads resources from files in the assets
    directory of the project.*/
class AndroidBank : public Bank
{
public:
    AndroidBank();
    virtual ~AndroidBank();
    
    std::string base_path;
    
    void setEnv(JNIEnv* env);
    
    virtual void initPersistentSerializableWithKey(Serializable* s, const char* key);
    virtual void writePersistentSerializableWithKey(const Serializable* s, const char* key);
    
    virtual void initDataWithPath(Data* s, const char* path);
    virtual void initSerializableWithPath(Serializable* s, const char* path);
    virtual void writeSerializableToPath(const Serializable* s, const char* path);
    virtual void initTextureWithPath(Texture2D* texture, const char* path);
    virtual void initBitmapWithPath(Bitmap* bitmap, const char* path);

protected:
    JNIEnv* getEnvForCurrentThread();

    std::string directory;
    
    JavaVM* jvm;
};

}

#endif

