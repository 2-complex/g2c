#include "renderergl1.h"

RendererGL1::RendererGL1()
{
}

RendererGL1::~RendererGL1()
{
}

void RendererGL1::init()
{
}

void RendererGL1::drawMesh(const Mesh* mesh,
                           const Mat4& matrix,
                           const Mat3& texMatrix,
                           const Color& color,
                           const Texture* texture) const
{
#if !defined(STUB_GL1)
    if( !mesh )
        mesh = quad;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    for(int i = 0; i < 16; i++)
        fv[i] = (GLfloat)(projection.ptr()[i]);
    glLoadMatrixf(fv);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    if( texture )
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture->getIndex());
    }
    else
        glDisable(GL_TEXTURE_2D);

    for(int i = 0; i < 16; i++)
        fv[i] = (GLfloat)(matrix.ptr()[i]);
    glLoadMatrixf(fv);

    glColor4f(color.r, color.g, color.b, color.a);

    GLuint gltype = GL_TRIANGLES;

    int dimension = 3; // Dimension of simplices, not space.
    if( mesh->elementType == Mesh::kLines )
    {
        gltype = GL_LINES;
        dimension = 2;
    }

    const unsigned short* indices = &(mesh->indices[0]);
    const float* positions = &(mesh->positions[0]);

    // Determine the size of the positions array from
    // the maximum index in indices.
    int size = 0;
    for(int e = 0; e < dimension * mesh->numberOfElements(); e++)
    {
        int i = indices[e];
        if( i > size )
            size = i;
    }
    size++;

    float* texCoords = new float[2*size];

    int j = 0;
    for(int i = 0; i < size; i++)
    {
        Vec3 c = texMatrix * Vec3(positions[3*i], positions[3*i+1], 1.0);

        texCoords[j] = c.x / c.z;
        texCoords[j+1] = c.y / c.z;

        j+=2;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, positions);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    glDrawElements(gltype, dimension * mesh->numberOfElements(), GL_UNSIGNED_SHORT, indices);

    delete[] texCoords;

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
#endif
}
