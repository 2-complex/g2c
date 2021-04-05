
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

#ifndef _SPRITES_
#define _SPRITES_

namespace g2c
{
    /*! An implementation of Renderer to draw meshes using OpenGL calls from the OpenGL ES 1
        collection.  The draw function in this will work in desktop OpenGL or in OpenGL ES 1.

        Set Mesh::renderer to an instance of RendererGL1 to and call init().  Then all meshes
        will draw using it.*/
    class RendererGL1 : public Renderer
    {
    public:
        RendererGL1();
        ~RendererGL1();

        virtual void init();
        virtual void drawMesh(
            const Mesh* mesh,
            const Mat4& matrix,
            const Mat3& texMatrix,
            const Color& color,
            const Texture* texture) const;
    };
}