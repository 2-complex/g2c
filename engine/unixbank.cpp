#include "unixbank.h"
#include "util.h"

#include "log.h"

#include <png.h>

using namespace std;

namespace cello
{

void UnixBank::initPersistentSerializableWithKey(Serializable* s, const char* key)
{
    initSerializableWithPath(s, (string(key) + ".txt").c_str());
}

void UnixBank::writePersistentSerializableWithKey(const Serializable* s, const char* key)
{
    writeSerializableToPath(s, (string(key) + ".txt").c_str());
}

void UnixBank::initDataWithPath(Data* data, const char* path)
{
    string fullpath = base_path + directory + path;
    
    FILE* fp = fopen(fullpath.c_str(), "r");
    if( !fp )
    {
        celloerror( "File not found: %s\n", fullpath.c_str() );
        exit(0);
    }
    
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    data->resize(size+1);
    data->array()[size] = 0;
    fread(data->array(), 1, size, fp);
    fclose(fp);
}

void UnixBank::initSerializableWithPath(Serializable* s, const char* path)
{
    string fullpath = base_path + directory + path;
    
    FILE* fp = fopen(fullpath.c_str(), "r");
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

void UnixBank::writeSerializableToPath(const Serializable* s, const char* path)
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

void UnixBank::initTextureWithPath(Texture2D* texture, const char* path)
{
    Bitmap bitmap;
    initBitmapWithPath(&bitmap, path);
    texture->initWithBitmap(bitmap);

}

static void abort_(const string& message)
{
    celloerror("%s\n", message.c_str());
    exit(0);
}

void UnixBank::initBitmapWithPath(Bitmap* bitmap, const char* path)
{
    string newpath = directory + path;

    int x, y;

    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep * row_pointers;

    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(newpath.c_str(), "rb");
    if (!fp)
        abort_(string("[read_png_file] File could not be opened for reading") + newpath);
    fread(header, 1, 8, fp);
    if (png_sig_cmp((png_bytep)header, 0, 8))
        abort_(string("[read_png_file] File %s is not recognized as a PNG file") + newpath);


    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if( ! png_ptr )
        abort_("[read_png_file] png_create_read_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
 
    if( ! info_ptr)
        abort_("[read_png_file] png_create_info_struct failed");

    if( setjmp(png_jmpbuf(png_ptr)) )
        abort_("[read_png_file] Error during init_io");

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);


    /* read file */

    int bpp = 4;
    if( color_type == 2 )
    {
        bpp = 3;
    }

    if( setjmp(png_jmpbuf(png_ptr)) )
        abort_("[read_png_file] Error during read_image");

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for( y=0; y<height; y++ )
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));		

    uint8_t* data = (GLubyte*)calloc(width * height * bpp, sizeof(uint8_t));

    png_read_image(png_ptr, row_pointers);

    for( y=0; y<height; y++ )
    {
        memcpy( data + bpp * width * (height - y - 1), row_pointers[y], sizeof(uint8_t) * bpp * width );
    }

    fclose(fp);

    bitmap->set(data, width, height, 8*bpp);

    free(data);
}

void UnixBank::initSoundWithPath(Sound* sound, const char* path)
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

} // end namespace

