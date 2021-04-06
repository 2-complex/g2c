#include "iosresourcebank.h"

#include <string>

using namespace std;

namespace cello
{


void IOSResourceBank::initPersistentSerializableWithKey(Serializable* s, const char* key)
{
	NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
	NSString* sString = [defaults objectForKey: [NSString stringWithUTF8String: key]];
	if( sString )
		s->deserialize(string([sString cStringUsingEncoding:1]));
}

void IOSResourceBank::writePersistentSerializableWithKey(const Serializable* s, const char* key)
{
	NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
	[defaults setObject: [NSString stringWithUTF8String:s->serialize().c_str()]
				forKey: [NSString stringWithUTF8String:key]];
	[defaults synchronize];
}

void IOSResourceBank::initSerializableWithPath(Serializable* s, const char* path)
{
	NSString *nsbundlepath = [[NSBundle mainBundle] bundlePath];
	string old_base_path = base_path;
	base_path = string([nsbundlepath cStringUsingEncoding:1]) + string("/");
	MacBank::initSerializableWithPath(s, path);
	base_path = old_base_path;
}

void IOSResourceBank::writeSerializableToPath(const Serializable* s, const char* path)
{
	NSString *nsbundlepath = [[NSBundle mainBundle] bundlePath];
	string revisedpath = string([nsbundlepath cStringUsingEncoding:1]) + string("/") + path;
	MacBank::writeSerializableToPath(s, revisedpath.c_str());
}

void IOSResourceBank::initSoundWithPath(Sound* sound, const char* path)
{
	NSString *nsbundlepath = [[NSBundle mainBundle] bundlePath];
	string old_base_path = base_path;
	base_path = string([nsbundlepath cStringUsingEncoding:1]) + string("/");
	MacBank::initSoundWithPath(sound, path);
	base_path = old_base_path;
}

void IOSResourceBank::initBitmapWithPath(Bitmap* bitmap, const char* path)
{
	string revisedpath = directory + path;
	CGImageRef image = [UIImage imageNamed:
		[NSString stringWithUTF8String:revisedpath.c_str()]].CGImage;
	if( !image )
	{
		celloerror( "image %s not found\n", revisedpath.c_str() );
		exit(0);
	}
	MacBank::initBitmapWithCGImage(bitmap, image);
}

} // end namespace


