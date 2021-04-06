#ifndef _FATTENING_
#define _FATTENING_

#include <string>

#include "graphics.h"
#include "sprites.h"

#include <stdint.h>
#include <vector>


namespace cello
{
    struct FatteningGeometryInfo
    {
        Geometry* geometry;
        Buffer* buffer;
        IndexBuffer* indexBuffer;
    };

    class Fattening
    {
    public:
        std::vector<double> position;
        std::vector<double> texcoord;
        std::vector<int> indices;

        void clear();
        void add(const Vec2& v, const Vec2& u);
        void add(int a, int b, int c);

        void fatten(const Polygon& polygon, double r);
        FatteningGeometryInfo newGeometry(const std::string& name);
        void populateModel(Model* model, Shape* shape);
    };

} // end namespace


#endif

