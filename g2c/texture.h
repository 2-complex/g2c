
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


#ifndef _TEXTURE_
#define _TEXTURE_

#include <string>

#include "opengl.h"
#include "element.h"

#include <stdint.h>

namespace g2c {

class Bank;
class Model;
class Bitmap;

class Bitmap {
public:
    Bitmap();
    Bitmap(const Bitmap& b);
    virtual ~Bitmap();
    
    Bitmap& operator=(const Bitmap& b);
    
    void set(uint8_t* inData, int inWidth, int inHeight, int inBitsPerPixel);
    void flipVertically();
    void swizzleRGB();
    void resize(int inWidth, int inHeight);

    const uint8_t* const getData() const;
    int getWidth() const;
    int getHeight() const;
    int getBitsPerPixel() const;

private:
    uint8_t* data;
    int width;
    int height;
    int bitsPerPixel;
    
    void mimmic(const Bitmap& b);
    void sample(double x, double y, double* color) const;
};


class Texture : public Element {
protected:
    Texture(GLenum target);
    Texture(GLenum target, int unit);
    virtual ~Texture();
    
    bool mipmaps;

public:
    GLuint getUnit() const;
    GLuint getIndex() const;
    GLuint getTarget() const;
    
    virtual std::string serializeElements(std::string indent = "") const;
    virtual void handleChild(const parse::Node* n);
    
    int bitsPerPixel;
    
    GLuint index;
    GLuint unit;
    GLenum target;
};


class Texture2D : public Texture {
friend class Value;
public:
    Texture2D();
    Texture2D(int unit);
    virtual ~Texture2D();
    
    GLubyte* data;
    int width;
    int height;
    
    std::string file;
    
    void initWithImageData(const GLubyte* inData,
                           int inWidth,
                           int inHeight,
                           int inBitsPerPixel);
    
    void initWithBitmap(const Bitmap& bitmap);
    
    virtual std::string serializeElements(std::string indent = "") const;
    virtual void handleChild(const parse::Node* n);

};


class CubeMap : public Texture {
friend class Value;
public:
    CubeMap();
    CubeMap(int unit);
    virtual ~CubeMap();
    
    int width;
    
    std::string positiveXFile;
    std::string negativeXFile;
    std::string positiveYFile;
    std::string negativeYFile;
    std::string positiveZFile;
    std::string negativeZFile;
    
    void initWithImageData(const GLubyte* positiveXData,
                           const GLubyte* negativeXData,
                           const GLubyte* positiveYData,
                           const GLubyte* negativeYData,
                           const GLubyte* positiveZData,
                           const GLubyte* negativeZData,
                           int inWidth,
                           int inBitsPerPixel);
    
    void initWithBitmaps(const Bitmap& bitmapPositiveX,
                         const Bitmap& bitmapNegativeX,
                         const Bitmap& bitmapPositiveY,
                         const Bitmap& bitmapNegativeY,
                         const Bitmap& bitmapPositiveZ,
                         const Bitmap& bitmapNegativeZ);
    
protected:
    virtual std::string serializeElements(std::string indent = "") const;
    virtual void handleChild(const parse::Node* n);
};

} // end namespace


#endif

