#include "bank.h"
#include "util.h"

#include <stdlib.h>

using namespace std;

namespace cello
{

Data::Data() : mArray(NULL), mSize(0) {}

Data::~Data()
{
    if( mArray )
        free(mArray);
}

void Data::resize(size_t size)
{
    if( mArray )
    {
        free(mArray);
        mArray = NULL;
    }
    
    if( size > 0 )
    {
        mArray = (uint8_t*)malloc(size);
    }
    
    mSize = size;
}

uint8_t* Data::array() const
{
    return mArray;
}

size_t Data::size() const
{
    return mSize;
}

void Bank::initSoundWithPath(Sound* sound, const char* path)
{
    if( endsWith(path, ".wav") || endsWith(path, ".wave") ||
        endsWith(path, ".WAV") || endsWith(path, ".WAVE") )
    {
    	Data data;
        Wave wave;
        
        initDataWithPath(&data, path);
        
        wave.initWithData(data.array(), data.size());
        
        sound->initWithWave(wave);
    }
}

void AsynchronousBank::initPersistentSerializableWithKey(Serializable* s, const char* key)
{
    bank->initPersistentSerializableWithKey(s, key);
}

void AsynchronousBank::writePersistentSerializableWithKey(const Serializable* s, const char* key)
{
    bank->writePersistentSerializableWithKey(s, key);
}

void AsynchronousBank::initSerializableWithPath(Serializable* s, const char* path)
{
    q.push(LoadInstruction(s, path));
}

void AsynchronousBank::writeSerializableToPath(const Serializable* s, const char* path)
{
    bank->writeSerializableToPath(s, path);
}

void AsynchronousBank::initSoundWithPath(Sound* sound, const char* path)
{
    q.push(LoadInstruction(sound, path));
}

void AsynchronousBank::initTextureWithPath(Texture2D* texture, const char* path)
{
    q.push(LoadInstruction(texture, path));
}

bool AsynchronousBank::step()
{
    if(!bank)
    {
        celloerror( "AsynchronousBank used with no bank set.\n" );
        exit(0);
    }
    
    if( q.empty() )
        return false;
    
    LoadInstruction inst = q.front();
    
    if( inst.resource->type == "Texture" ||
        inst.resource->type == "Texture2D" ||
        inst.resource->type == "Sampler" ||
        inst.resource->type == "Sprite" ||
        inst.resource->type == "Font" )
    {
        bank->initTextureWithPath((Texture2D*)inst.resource, inst.path.c_str());
    }
    else if( inst.resource->type == "Sound" )
    {
        bank->initSoundWithPath((Sound*)inst.resource, inst.path.c_str());
    }
    else
    {   // Assume if it's anything else it's a generic serializable.
        bank->initSerializableWithPath(inst.resource, inst.path.c_str());
    }
    q.pop();
    
    return q.empty();
}

int AsynchronousBank::percent() const
{
    int size = q.size();
    if(size > biggestSize)
        biggestSize = size;
    return (int)(100 * (1.0 - (1.0 * size / biggestSize)));
}

} // end namespace


