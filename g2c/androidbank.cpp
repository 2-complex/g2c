
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


#include "androidbank.h"
#include "util.h"

#include <android/log.h>
#include <android/bitmap.h>

#include <assert.h>
#include <string>

using namespace std;

#define  LOG_TAG    "android-bank"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


void AndroidBank::setEnvAndLoader(JNIEnv* env, jobject loader)
{
	this->env = env;
	this->loader = loader;
}

void AndroidBank::initPersistentSerializableWithKey(Serializable* s, const char* key)
{
	assert(false);
}

void AndroidBank::writePersistentSerializableWithKey(const Serializable* s, const char* key)
{
	assert(false);
}

void AndroidBank::initSerializableWithPath(Serializable* s, const char* path)
{
	string fullpath = base_path + directory + path;
	
	string old_directory = directory;
	directory += directoryOfPath(path);
	
	// Get the Loader class.
	jclass class_Loader = env->FindClass("com.android.gl2jni.Loader");
	
	// Get the c object representing the java method for getting a string.
	jmethodID method_getStringFromPath = env->GetMethodID(
		class_Loader, "getStringFromPath", "(Ljava/lang/String;)Ljava/lang/String;");
	
	// Construct input to java function as a jstring.
	jstring java_path = env->NewStringUTF(fullpath.c_str());
	
	// Call loader java method to get text from file.
	jstring java_str = (jstring) env->CallObjectMethod(loader, method_getStringFromPath, java_path);
	
	// Convert to c string.
	const char *native_str = env->GetStringUTFChars(java_str, NULL);
	
	// Deserialize from c string.
	s->deserialize(native_str);
	
	// Deallocate c string.
	env->ReleaseStringUTFChars(java_str, native_str);
	
	directory = old_directory;
}

void AndroidBank::writeSerializableToPath(const Serializable* s, const char* path)
{
	assert(false);
}

void AndroidBank::initBitmapWithPath(Bitmap* bitmap, const char* path)
{
	string fullpath = base_path + directory + path;
	
	AndroidBitmapInfo info;
	void* pixels;
	int ret;
	
	// Get the Loader class.
	jclass class_Loader = env->FindClass("com.android.gl2jni.Loader");
    
    // Get the c object representing the java method for getting a bitmap.
    jmethodID method_getBitmapFromPath = env->GetMethodID(
		class_Loader, "getBitmapFromPath", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
	
	// Construct input to java function as a jstring.
	jstring java_path = env->NewStringUTF(fullpath.c_str());
	
    jobject java_bitmap = env->CallObjectMethod(
    	loader, method_getBitmapFromPath, java_path);
	
    if( (ret = AndroidBitmap_getInfo(env, java_bitmap, &info)) < 0 )
    {
        LOGE("AndroidBitmap_getInfo failed. error=%d", ret);
        return;
    }
	
    if( info.format != ANDROID_BITMAP_FORMAT_RGBA_8888 )
    {
        LOGE("Bitmap format is not ANDROID_BITMAP_FORMAT_RGBA_8888 is %d !", info.format);
        return;
    }
	
    if( (ret = AndroidBitmap_lockPixels(env, java_bitmap, &pixels)) < 0 )
    {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return;
    }
	
	bitmap->set(
		(uint8_t*)pixels,
		info.width,
		info.height,
		32);
	
	if( (ret = AndroidBitmap_unlockPixels(env, java_bitmap)) < 0 )
    {
        LOGE("AndroidBitmap_unlockPixels() failed ! error=%d", ret);
        return;
    }
	
	bitmap->flipVertically();
	// bitmap->swizzleRGB();
}

void AndroidBank::initTextureWithPath(Texture2D* texture, const char* path)
{
	Bitmap bitmap;
	initBitmapWithPath(&bitmap, path);
	texture->initWithBitmap(&bitmap);
}

#if !defined(STUB_SOUND)
void AndroidBank::initSoundWithPath(Sound* sound, const char* path)
{
	
}

void* AndroidBank::getOpenALAudioData(CFURLRef inFileURL,
								  ALsizei* outDataSize,
								  ALenum* outDataFormat,
								  ALsizei* outSampleRate) const
{
	
}
#endif
