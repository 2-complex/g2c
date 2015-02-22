
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


#include "emscriptenbank.h"
#include "util.h"
#include "log.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>


using namespace std;

namespace g2c {

void EmscriptenBank::initPersistentSerializableWithKey(Serializable* s, const char* key)
{
}

void EmscriptenBank::writePersistentSerializableWithKey(const Serializable* s, const char* key)
{
}

void EmscriptenBank::initDataWithPath(Data* data, const char* path)
{
}

void EmscriptenBank::initSerializableWithPath(Serializable* s, const char* path)
{
    string fullpath = base_path + directory + path;
}

void EmscriptenBank::writeSerializableToPath(const Serializable* s, const char* path)
{
    FILE* fp = fopen(path, "w");
    if( !fp )
    {
        g2cerror( "serializable file failed to open for writing: %s\n", path );
    }
    string data = s->serialize();
    fwrite(data.c_str(), data.size(), 1, fp);
    fclose(fp);
}

void EmscriptenBank::initTextureWithPath(Texture2D* texture, const char* path)
{
    uint8_t defaultTexturePixels[] = {
        0x55, 0x00, 0xff, 0xff,
        0x55, 0x00, 0x00, 0xff,
        0x55, 0x00, 0x00, 0xff,
        0x55, 0x00, 0xff, 0xff};

    texture->initWithImageData(
        (const GLubyte *)(defaultTexturePixels), 2, 2, 32);

    string command = "Bank.initTextureWithPath("
        + stringRepr(path) + ", "
        + intToString(texture->getIndex()) + ", "
        + string(texture->mipmaps ? "true" : "false") + ")";

    emscripten_run_script(command.c_str());
}


void EmscriptenBank::initBitmapWithPath(Bitmap* bitmap, const char* path)
{
    string newpath = directory + path;
}

void EmscriptenBank::initSoundWithPath(Sound* sound, const char* path)
{
}

} // end namespace

