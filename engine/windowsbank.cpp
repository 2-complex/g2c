#include "windowsbank.h"
#include "util.h"


#include <windows.h>
#include <objidl.h>

// gdiplus.h assumes min and max exist, workaround is to include algorithm
// where they're defined, then use the namespace before including gdiplus.h.
#include <algorithm>
using namespace std;
#include <gdiplus.h>

namespace cello
{




void WindowsBank::initPersistentSerializableWithKey(Serializable* s, const char* key)
{
    initSerializableWithPath(s, (string(key) + ".txt").c_str());
}

void WindowsBank::writePersistentSerializableWithKey(const Serializable* s, const char* key)
{
    writeSerializableToPath(s, (string(key) + ".txt").c_str());
}

void WindowsBank::initSerializableWithPath(Serializable* s, const char* path)
{
    string fullpath = base_path + directory + path;
    
    FILE* fp = fopen(fullpath.c_str(), "rb");
    if( !fp )
    {
        celloerror( "serializable file not found: %s\n", fullpath.c_str() );
        exit(0);
    }
    
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* data = (char*)malloc(size+1);
    data[size] = 0;
    fread(data, 1, size, fp);
    fclose(fp);
    
    string old_directory = directory;
    directory += directoryOfPath(path);
    s->deserialize(data);
    directory = old_directory;
    
    free(data);
}

void WindowsBank::writeSerializableToPath(const Serializable* s, const char* path)
{
    FILE* fp = fopen(path, "w");
    if( !fp )
    {
        celloerror( "serializable file failed to open for writing: %s\n", path );
        exit(0);
    }
    string data = s->serialize();
    fwrite(data.c_str(), data.size(), 1, fp);
    fclose(fp);
}

void WindowsBank::initBitmapWithPath(Bitmap* bitmap, const char* path)
{
    string newpath = base_path + directory + path;

    int n = newpath.size();
    WCHAR* wPath = new WCHAR[n+1];
    for( int i = 0; i < n; i++ )
    {
        char c = newpath[i];
        wPath[i] = ((c=='/') ? '\\' : c);
    }
    wPath[n] = 0;

    Gdiplus::Bitmap* gdiBitmap = Gdiplus::Bitmap::FromFile(wPath, false);
    
    if( !gdiBitmap )
    {
        celloerror( "Image file failed to open: %s\n", path );
        exit(0);
    }

    delete[] wPath;
    
    Gdiplus::Rect fullrect(0, 0, gdiBitmap->GetWidth(), gdiBitmap->GetHeight());

    Gdiplus::BitmapData bitmapData;
    gdiBitmap->LockBits(
        &fullrect,
        Gdiplus::ImageLockModeWrite,
        PixelFormat32bppARGB,
        &bitmapData);
    
    bitmap->set((uint8_t*)bitmapData.Scan0,
        bitmapData.Width,
        bitmapData.Height,
        32);

    bitmap->flipVertically();
    bitmap->swizzleRGB();

    gdiBitmap->UnlockBits(&bitmapData);

    delete gdiBitmap;
}

void WindowsBank::initTextureWithPath(Texture2D* texture, const char* path)
{
    Bitmap bitmap;
    initBitmapWithPath(&bitmap, path);
    texture->initWithBitmap(&bitmap);
}

void WindowsBank::initSoundWithPath(Sound* sound, const char* path)
{

}

} // end namespace
