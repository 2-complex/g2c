#ifndef _SPRITES_
#define _SPRITES_

namespace cello
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