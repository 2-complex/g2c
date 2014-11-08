
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


#include "sprites.h"

#include <assert.h>


using namespace std;

namespace g2c {

bool Sprite::drawLines = false; // Some time put this in the renderer, perhaps.
Renderer* Mesh::renderer = NULL;


Mat4Property::Mat4Property() {}
Mat4Property::Mat4Property(const Mat4& v) : Mat4(v) {}

std::string Mat4Property::serialize(std::string indent) const
{
    VectorProperty<DoubleProperty> v;
    const double* p = ptr();
    for( int i = 0; i < 16; i++ )
        v.push_back(p[i]);

    return flattenWhitespace(v.serialize(indent).c_str());
}

void Mat4Property::initWithParseNode(const parse::Node* n)
{
    VectorProperty<DoubleProperty> v;
    v.initWithParseNode(n);

    double* ptr = (double*)(&m00);
    int size = v.size();

    for( int i = 0; i < 16; i++ )
        if( size > 0 )
            ptr[i] = v[i]();
}


Vec2Property::Vec2Property() {}
Vec2Property::Vec2Property(const Vec2& v) : Vec2(v) {}

std::string Vec2Property::serialize(std::string indent) const
{
    VectorProperty<DoubleProperty> v;
    v.push_back(x);
    v.push_back(y);
    return flattenWhitespace(v.serialize(indent).c_str());
}

void Vec2Property::initWithParseNode(const parse::Node* n)
{
    VectorProperty<DoubleProperty> v;
    v.initWithParseNode(n);

    if( v.size() > 0 )
        x = v[0]();

    if( v.size() > 1 )
        y = v[1]();
}


Vec3Property::Vec3Property() {}
Vec3Property::Vec3Property(const Vec3& v) : Vec3(v) {}

std::string Vec3Property::serialize(std::string indent) const
{
    VectorProperty<DoubleProperty> v;
    v.push_back(x);
    v.push_back(y);
    v.push_back(z);
    return flattenWhitespace(v.serialize(indent).c_str());
}

void Vec3Property::initWithParseNode(const parse::Node* n)
{
    VectorProperty<DoubleProperty> v;
    v.initWithParseNode(n);

    if( v.size() > 0 )
        x = v[0]();

    if( v.size() > 1 )
        y = v[1]();

    if( v.size() > 2 )
        z = v[2]();
}


ColorProperty::ColorProperty() {}
ColorProperty::ColorProperty(const Color& c) : Color(c) {}

std::string ColorProperty::serialize(std::string indent) const
{
    VectorProperty<DoubleProperty> v;
    v.push_back(r);
    v.push_back(g);
    v.push_back(b);
    v.push_back(a);
    return flattenWhitespace(v.serialize(indent).c_str());
}

void ColorProperty::initWithParseNode(const parse::Node* n)
{
    VectorProperty<DoubleProperty> v;
    v.initWithParseNode(n);

    if( v.size() > 0 )
        r = v[0]();

    if( v.size() > 1 )
        g = v[1]();

    if( v.size() > 2 )
        b = v[2]();

    if( v.size() > 3 )
        a = v[3]();
}

Sampler::Sampler()
{
    type = "Sampler";
    addProperty("file", file);
}

Sampler::~Sampler()
{
}

Mat4 Sampler::getOffsetMatrix(int frame, double x, double y, double k) const
{
    double w = getWidth(frame),
           h = getHeight(frame);

    Vec2 offset(x, y);
    if( getCenter(frame) )
        offset -= 0.5 * k * Vec2(w, h);

    return Mat4(
        k*w, 0.0, 0.0, 0.0,
        0.0, k*h, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        offset.x, offset.y, 0.0, 1.0);
}

Rectangle::Rectangle()
    : x0(0.0)
    , y0(0.0)
    , x1(1.0)
    , y1(1.0)
{
}

Rectangle::Rectangle(double x0, double y0, double x1, double y1)
    : x0(x0)
    , y0(y0)
    , x1(x1)
    , y1(y1)
{
}

Rectangle::~Rectangle()
{
}

std::string Rectangle::serialize(std::string indent) const
{
    VectorProperty<DoubleProperty> v;
    v.push_back(x0);
    v.push_back(y0);
    v.push_back(x1);
    v.push_back(y1);
    return flattenWhitespace(v.serialize(indent).c_str());
}

void Rectangle::initWithParseNode(const parse::Node* n)
{
    VectorProperty<DoubleProperty> v;
    v.initWithParseNode(n);

    if( v.size() > 0 )
        x0 = v[0]();

    if( v.size() > 1 )
        y0 = v[1]();

    if( v.size() > 2 )
        x1 = v[2]();

    if( v.size() > 3 )
        y1 = v[3]();
}

Sprite::Sprite()
    : numberOfRows(1)
    , numberOfColumns(1)
    , center(false)
    , flipRows(false)
{
    type = "Sprite";

    addProperty("numberOfRows", numberOfRows);
    addProperty("numberOfColumns", numberOfColumns);
    addProperty("center", center);
    addProperty("flipRows", flipRows);
    addProperty("polygon", polygon);
}

Sprite::~Sprite()
{
}

Mat3 Sprite::getTexMatrix(int frame) const
{
    int c = numberOfColumns();
    int r = numberOfRows();

    double i = (frame % c + c) % c,
           j = ((frame / c) % r + r) % r;

    if( flipRows() )
        j = numberOfRows() - j - 1;

    return Mat3(
        1.0 / c, 0.0     , 0.0,
                 0.0, 1.0 / r, 0.0,
                i / c, j / r, 1.0);
}

double Sprite::getWidth(int frame) const
{
    return width / numberOfColumns();
}

double Sprite::getHeight(int frame) const
{
    return height / numberOfRows();
}

bool Sprite::getCenter(int frame) const
{
    return center;
}

Polygon Sprite::getCollisionPolygon(int frame) const
{
    return polygon;
}

void Sprite::handleChild(const parse::Node* n)
{
    string n_name = n->data.s;
    // parse::Node* value = n->data.value;

    Serializable::handleChild(n);
}

string Sprite::serializeElements(string indent) const
{
    string r = Serializable::serializeElements(indent);
    return r;
}

Atlas::Atlas()
{
    type = "Atlas";
    addProperty("rectangles", rectangles);
}

Atlas::~Atlas()
{
}

Mat3 Atlas::getTexMatrix(int frame) const
{
    int n = rectangles.size();

    if( n == 0 )
    {
        g2cerror("Atlas used with no rectangles.  Returning idenitity matrix.");
        return Mat3();
    }

    frame = ( ( frame % n ) + n ) % n;
    const Rectangle& rectangle( rectangles[frame] );

    // Matrix that transforms the unit square to the rectangle
    // in texture [0,0,1,1] coordinates.
    Mat3 result(
        (rectangle.x1 - rectangle.x0) / width, 0.0, 0.0,
        0.0, (rectangle.y1 - rectangle.y0) / height, 0.0,
        rectangle.x0 / width, rectangle.y0 / height, 1.0);

    return result;
}

double Atlas::getWidth(int frame) const
{
    int n = rectangles.size();

    if( n == 0 )
    {
        g2cerror("Atlas used with no rectangles.  Returning width 10.");
        return 10.0;
    }

    frame = ( ( frame % n ) + n ) % n;
    const Rectangle& rectangle( rectangles[frame] );

    return fabs(rectangle.x1 - rectangle.x0);
}

double Atlas::getHeight(int frame) const
{
    int n = rectangles.size();

    if( n == 0 )
    {
        g2cerror("Atlas used with no rectangles.  Returning height 10.");
        return 10.0;
    }

    frame = ( ( frame % n ) + n ) % n;
    const Rectangle& rectangle( rectangles[frame] );

    return fabs(rectangle.y1 - rectangle.y0);
}

bool Atlas::getCenter(int frame) const
{
    return center;
}

Polygon Atlas::getCollisionPolygon(int frame) const
{
    int n = rectangles.size();

    if( n == 0 )
    {
        g2cerror("Atlas used with no rectangles.  Returning width empty polygon.");
        return Polygon();
    }

    frame = ( ( frame % n ) + n ) % n;

    return polygons[frame];
}


Font::Font() : widths(128), lineHeight(32.0), lineBottom(16.0), widthScale(1.0), spacing(0.0), baseChar(' ')
{
    type = "Font";
    for(int i = 0; i < 128; i++)
        widths[i] = 32.0;
}

double Font::charWidth(char c) const
{
    int index = c-baseChar;
    int n = widths.size();
    if( index < 0 || index >= n )
    {
        g2cerror( "charWidth: Attempt to access off "
                "the edge of widths array: %d\n", c );
        exit(0);
    }
    return widthScale * (widths[index] + spacing);
}

double Font::charLeft(char c) const
{
    int index = c-baseChar;
    int n = lefts.size();
    if( index < 0 || index >= n )
        return 0.0;

    return widthScale * lefts[index];
}


double Font::lineWidth(double k, const char* s, int startIndex) const
{
    double width = 0;

    char c = 0;
    for(int i = startIndex; (c=s[i]); i++)
    {
        if( c == '\n' || c == '\r' )
            break;
        width += charWidth(c);
    }
    return k * width;
}

vector<pair<int, int> > Font::lineIndices(const char* s) const
{
    vector<pair<int, int> > v;

    int first = 0;
    int i=0;
    char c=0;

    for(; (c=s[i]); i++)
    {
        if( c == '\n' || c == '\r' )
        {
            v.push_back(pair<int, int>(first, i));
            first = i+1;
        }
    }
    // If there's just a newline character with nothing else
    // act like there isn't a last line.
    if(first != i)
        v.push_back(pair<int, int>(first, i));

    return v;
}

void Font::drawString(const Mat4& M,
                      const Color& color,
                      double x,
                      double y,
                      double k,
                      const char* s,
                      const string& justification) const
{
    double startX = x;
    vector<pair<int, int> > v = lineIndices(s);
    for(vector<pair<int, int> >::const_iterator itr = v.begin(); itr!=v.end(); itr++)
    {
        double w = lineWidth(k, s, itr->first);
        x = startX;

        if( justification == "right" )
            x -= w;

        if( justification == "center" )
            x -= 0.5 * w;

        for(int i=itr->first; i<itr->second; i++)
        {
            char c = s[i];
            double l = k * charLeft(c);

            Mat4 matrix(M * getOffsetMatrix(c-baseChar, x-l, y, k));
            Mat3 texMatrix(getTexMatrix(c-baseChar));

            Mesh::renderer->drawMesh(NULL, matrix, texMatrix, color, this);

            x += k * charWidth(c);
        }
        y -= k * lineHeight;
    }
}

Polygon Font::stringRectangle(double k, const char* s, const string& justification) const
{
    double left = 0.0, right = 0.0, bottom = 0.0,
        top = k * (lineHeight + lineBottom);

    vector<pair<int, int> > v = lineIndices(s);
    bool firstline = true;
    for(vector<pair<int, int> >::const_iterator itr = v.begin(); itr!=v.end(); itr++)
    {
        double w = lineWidth(k, s, itr->first);
        double x = 0;

        if( justification == "right" )
            x -= w;

        if( justification == "center" )
            x -= 0.5 * w;

        left = min(left, x);
        right = max(right, x+w);

        if( !firstline )
            bottom -= k * lineHeight;

        firstline = false;
    }

    Polygon R;
    R.add(left, bottom).add(right, bottom).add(right, top).add(left, top);
    R.setDrawType(Polygon::kOutline);
    return R;
}

void Font::getWidthsFromBitmap(const Bitmap& bitmap)
{
    const uint8_t* data = bitmap.getData();
    double width = bitmap.getWidth();
    double height = bitmap.getHeight();
    int bitsPerPixel = bitmap.getBitsPerPixel();

    double w = 1.0 * width / numberOfColumns;
    double h = 1.0 * height / numberOfRows;

    if( bitsPerPixel != 32 )
    {
        g2clog("Attempt to generate widths, not from 32 bit data.");
        return;
    }

    widths.clear();
    lefts.clear();

    double threshold = 255.0;

    if( numberOfRows == 1 && numberOfColumns == 1 )
    {
        g2clog("Attempt to generate widths with"
               " numberOfRows = numberOfColumns = 1.\n");
    }

    vector<double> sums((int)(w+1));

    for( int j = 0; j < numberOfRows; j++ )
    for( int i = 0; i < numberOfColumns; i++ )
    {
        int x0 = min((int)((i)*w), width);
        int x1 = min((int)((i+1)*w), width);
        int y0 = min((int)(j*h), height);
        int y1 = min((int)((j+1)*h), height);

        /* iterate through find the first column the sum of whose
           alpha is greater than threshold and the last that is less */
        for( int x = x0; x < x1; x++ )
        {
            double sum = 0;
            for( int y = y0; y < y1; y++ )
            {
                int index = (bitsPerPixel / 8) * (width * y + x);
                uint8_t a = data[index + 3];
                sum += 1.0 * a;
            }
            sums[x-x0] = sum;
        }

        double right = w / 2;
        double left = 0;

        for( int x = x0; x < x1; x++ )
        {
            if(sums[x-x0] > threshold)
            {
                left = x-x0;
                break;
            }
        }

        for( int x = x1-1; x >= x0; x-- )
        {
            if(sums[x-x0] > threshold)
            {
                right = x-x0;
                break;
            }
        }

        widths.push_back(right - left);
        lefts.push_back(left);
    }

    spacing = 3;
}

string Font::serializeElements(string indent) const
{
    string r = Sprite::serializeElements(indent);

    r += TAB + indent + "'lineHeight' : " + floatToString(lineHeight) + ",\n";
    r += TAB + indent + "'lineBottom' : " + floatToString(lineBottom) + ",\n";
    if( widthScale!=1.0 )
        r += TAB + indent + "'widthScale' : " + floatToString(widthScale) + ",\n";

    if( spacing )
        r += TAB + indent + "'spacing' : " + floatToString(spacing) + ",\n";

    string s;
    int n = 0;

    n = widths.size();
    s = "[";
    for(int i = 0; i < n; i++)
    {
        if( i % numberOfColumns() == 0 )
            s += "\n" + TAB + TAB + TAB + indent;
        s += floatToString(widths[i]);
        if( i!=n-1 )
            s += ", ";
    }
    s += "\n" + TAB + TAB + indent + "]";
    r += TAB + indent + "'widths' : " + s + string(",\n");

    n = lefts.size();
    s = "[";
    for(int i = 0; i < n; i++)
    {
        if( i % numberOfColumns() == 0 )
            s += "\n" + TAB + TAB + TAB + indent;
        s += floatToString(lefts[i]);
        if( i!=n-1 )
            s += ", ";
    }
    s += "\n" + TAB + TAB + indent + "]";
    r += TAB + indent + "'lefts' : " + s + string(",\n");

    if(baseChar!=' ')
    {
        char c[2] = {baseChar, 0};
        r += TAB + indent + "'baseChar' : '" + string(c) + "',\n";
    }

    return r;
}

void Font::handleChild(const parse::Node* n)
{
    Sprite::handleChild(n);

    string n_name = n->data.s;
    parse::Node* value = n->data.value;

    if(n_name == "widths")
    {
        widths.clear();
        for(vector<parse::Node*>::const_iterator itr = value->children.begin();
            itr!=value->children.end();
            itr++)
        {
            widths.push_back((*itr)->data.x);
        }
    }

    if(n_name == "lefts")
    {
        lefts.clear();
        for(vector<parse::Node*>::const_iterator itr = value->children.begin();
            itr!=value->children.end();
            itr++)
        {
            lefts.push_back((*itr)->data.x);
        }
    }

    if(n_name == "lineHeight")
    {
        lineHeight = value->data.x;
    }

    if(n_name == "lineBottom")
    {
        lineBottom = value->data.x;
    }

    if(n_name == "widthScale")
    {
        widthScale = value->data.x;
    }

    if(n_name == "spacing")
    {
        spacing = value->data.x;
    }

    if(n_name == "baseChar")
    {
        baseChar = value->data.s[0];
    }
}


Node::Node() : visible(true), parent(NULL), tookMouseDown(false)
{
    type = "Node";
    addProperty("visible", visible);
    addProperty("children", children);
}

Node::~Node()
{
    for(vector<Node*>::iterator itr = children.begin(); itr!=children.end(); itr++)
        delete *itr;
}

void Node::add(Node* t)
{
    children.push_back(t);
    t->parent = this;
}

void Node::addAtIndex(Node* t, int index)
{
    assert(t);

    int n = children.size();

    if(index < 0) index = 0;
    if(index > n) index = n;

    children.resize(n+1);
    int i = n;
    for(;i>index; i--)
        children[i] = children[i-1];
    children[i] = t;
    t->parent = this;
}

int Node::indexOf(const Node* t) const
{
    int n = children.size();
    for(int i = 0; i < n; i++)
        if( children[i]==t )
            return i;
    return -1;
}

void Node::remove(Node* t)
{
    int n = children.size();
    int j = 0;
    for(int i = 0; i < n; i++)
    {
        if(children[i] != t)
        {
            children[j] = children[i];
            j++;
        }
    }
    children.resize(j);
    t->parent = NULL;
}

void Node::clear()
{
    children.clear();
}

void Node::removeAndDelete(Node* t)
{
    remove(t);
    delete t;
}

void Node::removeSampler(const Sampler* sampler)
{
    for(vector<Node*>::const_iterator itr = children.begin(); itr!=children.end(); itr++)
        (*itr)->removeSampler(sampler);
}

Node* Node::findChild(const string& name) const
{
    for(vector<Node*>::const_iterator itr = children.begin(); itr!=children.end(); itr++)
    {
        if( (*itr)->name == name )
            return *itr;
    }
    g2clog( "WARNING: child %s of node %s not found.\n", name.c_str(), this->name.c_str() );
    return NULL;
}

void Node::draw() const
{
    if( visible() )
    {
        drawInTree(getMatrix(), getColor());
    }
}

void Node::drawInTree(const Mat4& worldMatrix, const Color& worldColor) const
{
    for(vector<Node*>::const_iterator itr = children.begin(); itr!=children.end(); itr++)
    {
        Node* node = *itr;

        if( node->visible() )
        {
            node->drawInTree(
                worldMatrix * node->getMatrix(),
                worldColor * node->getColor() );
        }
    }
}

void Node::handleChild(const parse::Node* n)
{
    const parse::Node* value = n->data.value;
    string n_name = n->data.s;

    if( n_name == "children" )
    {
        for(vector<parse::Node*>::const_iterator itr = value->children.begin();
            itr!=value->children.end();
            itr++)
        {
            string type = getType(*itr);
            Node* newNode = NULL;
            if(type=="Node" ||  type=="")
                newNode = new Node();
            else if(type == "World")
                newNode = new World();
            else if(type == "Layer")
                newNode = new Layer();
            else if(type == "Actor")
                newNode = new Actor();
            else if(type == "Button")
                newNode = new Button();
            else if(type == "String" || type == "Text") // String is deprecated.
                newNode = new Text();
            else if(type == "Integer")
                newNode = new Integer();
            else if(type == "Polygon")
                newNode = new Polygon();

            if(newNode)
            {
                newNode->initWithParseNode(*itr);
                add(newNode);
                deleteMe.push_back(newNode);
            }
        }
    }
    else
        Serializable::handleChild(n);
}

void Node::clearChildren()
{
    for( vector<Node*>::iterator itr = children.begin();
         itr != children.end();
         itr++ )
    {
        (*itr)->clearChildren();
    }

    children.clear();

    for( vector<Serializable*>::iterator itr = deleteMe.begin();
         itr!=deleteMe.end();
         itr++)
    {
        delete *itr;
    }

    deleteMe.clear();
}

bool Node::vectorInside( const Mat4& /*worldMatrix*/, const Vec2& /*V*/ ) const
{
    return true;
}

void Node::keyboard(unsigned char inkey)
{
    if( delegate )
        delegate->keyboard(inkey);
    else
    {
        for(vector<Node*>::reverse_iterator itr = children.rbegin();
            itr!=children.rend();
            itr++)
        {
            Node* node = *itr;
            if( node->listening )
            {
                (*itr)->keyboard(inkey);
            }
        }
    }
}

bool Node::mouseDown(const Vec2& C)
{
    return mouseDown(getMatrix(), C);
}

bool Node::mouseDown(const Mat4& worldMatrix, const Vec2& C)
{
    if( !visible() )
        return false;

    if( delegate && vectorInside(worldMatrix, C) )
    {
        Vec4 v = worldMatrix.inverse() * Vec4(C.x, C.y, 0.0, 1.0);
        v /= v.w;
        bool result = delegate->mouseDown(Vec2(v.x, v.y));
        if( result )
            tookMouseDown = true;
        return result;
    }
    else
    {
        for(vector<Node*>::reverse_iterator itr = children.rbegin();
            itr!=children.rend();
            itr++)
        {
            Node* node = *itr;

            if(node->listening)
            {
                if( node->mouseDown(worldMatrix * node->getMatrix(), C) )
                {
                    tookMouseDown = true;
                    return true;
                }
            }
        }
        return false;
    }
}

void Node::mouseDragged(const Vec2& C)
{
    mouseDragged(getMatrix(), C);
}

void Node::mouseDragged(const Mat4& worldMatrix, const Vec2& C)
{
    if(!tookMouseDown)
        return;

    if( delegate )
    {
        Vec4 v = worldMatrix.inverse()*Vec4(C.x, C.y, 0, 1);
        v /= v.w;
        delegate->mouseDragged(Vec2(v.x, v.y));
    }
    else
    {
        for(vector<Node*>::reverse_iterator itr = children.rbegin();
            itr!=children.rend();
            itr++)
        {
            Node* node = *itr;
            if(node->listening)
                node->mouseDragged(worldMatrix * node->getMatrix(), C);
        }
    }
}

void Node::mouseUp(const Vec2& C)
{
    // Default behavior same as mouseDragged.
    mouseDragged(C);
}

void Node::mouseUp(const Mat4& worldMatrix, const Vec2& C)
{
    if(!tookMouseDown)
        return;

    if( delegate )
    {
        Vec4 v = worldMatrix.inverse()*Vec4(C.x, C.y, 0, 1);
        v /= v.w;
        delegate->mouseUp(Vec2(v.x, v.y));
    }
    else
    {
        for(vector<Node*>::reverse_iterator itr = children.rbegin();
            itr!=children.rend();
            itr++)
        {
            Node* node = *itr;
            if(node->listening)
            {
                (*itr)->mouseUp(worldMatrix * node->getMatrix(), C);
            }
        }
    }
}

Mat4 Node::getMatrix() const
{
    return Mat4();
}

Color Node::getColor() const
{
    return color;
}

Mat4 Node::getWorldMatrix() const
{
    if(parent)
        return parent->getWorldMatrix() * getMatrix();

    return Mat4();
}

Color Node::getWorldColor() const
{
    if(parent)
        return parent->getWorldColor() * getColor();
    return Color(1.0, 1.0, 1.0, 1.0);
}

Actor* Node::actorInClick(const Mat4& worldMatrix, const Vec2& C)
{
    if(!visible())
        return NULL;

    for(vector<Node*>::reverse_iterator itr = children.rbegin(); itr!=children.rend(); itr++)
    {
        Actor* n = (*itr)->actorInClick(worldMatrix * getMatrix(), C);
        if( n )
            return n;
    }
    return NULL;
}

void Node::clearTookMouseDown()
{
    tookMouseDown = false;
    for(vector<Node*>::iterator itr = children.begin(); itr!=children.end(); itr++)
        (*itr)->clearTookMouseDown();
}


Mesh::Mesh() : elementType(kTriangles)
{
}

Mesh::~Mesh()
{
}

void Mesh::resize(int inNumberOfVertices, int inNumberOfElements)
{
    int sd = 3;
    if( elementType == kLines )
        sd = 2;

    positions.resize(inNumberOfVertices * 3);
    indices.resize(inNumberOfElements * sd);
}

int Mesh::numberOfVertices() const
{
    return positions.size() / 3;
}

int Mesh::numberOfElements() const
{
    int sd = 3;
    if( elementType == kLines )
        sd = 2;

    return indices.size() / sd;
}

Renderer::Renderer()
{
    quad = new Mesh;
    quad->elementType = Mesh::kTriangles;

    const float temp_positions[] = { 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0 };
    const unsigned short temp_indices[] = { 0, 1, 2,  0, 2, 3 };

    for(int i = 0; i < sizeof(temp_positions)/sizeof(float); i++)
    	quad->positions.push_back(temp_positions[i]);

    for(int i = 0; i < sizeof(temp_indices)/sizeof(unsigned short); i++)
        quad->indices.push_back(temp_indices[i]);
}

Renderer::~Renderer()
{
    delete quad;
}

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

RendererGL2::RendererGL2()
    : initialized(false)
    , program(0)
    , vertexShader(0)
    , fragmentShader(0)
    , defaultTexture(NULL)
{

}

RendererGL2::~RendererGL2()
{
    if( program )
        glDeleteProgram(program);
    if( vertexShader )
        glDeleteShader(vertexShader);
    if( fragmentShader )
        glDeleteShader(fragmentShader);

    if( defaultTexture )
        delete defaultTexture;
}

void RendererGL2::init()
{
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &(quad->positions[0]), GL_STREAM_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), &(quad->indices[0]), GL_STREAM_DRAW);

    glGenBuffers(1, &polygonBuffer);
    glGenBuffers(1, &polygonIndexBuffer);

    const char* vertexCode = "\n"
        "    attribute vec3 position;\n"
        "    uniform mat4 matrix;\n"
        "    uniform mat3 texMatrix;\n"
        "    varying vec2 v_texcoord;\n"
        "    void main() {\n"
        "        vec3 t = texMatrix * vec3(position.xy, 1.0);\n"
        "        v_texcoord = t.xy / t.z;\n"
        "        gl_Position = matrix * vec4(position, 1.0);\n"
        "    }\n";

    const char* fragmentCode = "\n"
        "#ifdef GL_ES\nprecision highp float;\n#endif\n"
        "    uniform vec4 color;\n"
        "    uniform sampler2D texture;\n"
        "    varying vec2 v_texcoord;\n"
        "    void main() {\n"
        "        gl_FragColor = color * texture2D(texture, v_texcoord);\n"
        "    }\n";

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);

    GLint logLength;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
    if( logLength > 0 )
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
        g2clog("Shader compile log:\n%s", log);
        free(log);
    }

    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if( status == 0 )
    {
        glDeleteShader(vertexShader);
        exit(0);
    }


    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
    if( logLength > 0 )
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(fragmentShader, logLength, &logLength, log);
        g2clog("Shader compile log:\n%s", log);
        free(log);
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if( status == 0 )
    {
        glDeleteShader(fragmentShader);
        exit(0);
    }

    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if( status == 0 )
    {
        g2cerror( "Shader program did not link.\n" );
        exit(0);
    }

    positionLocation = glGetAttribLocation(program, "position");
    matrixLocation = glGetUniformLocation(program, "matrix");
    colorLocation = glGetUniformLocation(program, "color");
    textureLocation = glGetUniformLocation(program, "texture");
    texMatrixLocation = glGetUniformLocation(program, "texMatrix");

    defaultTexture = new Texture2D;
    GLubyte data[] = {255, 255, 255, 255};
    defaultTexture->initWithImageData(data, 1, 1, 32);

    initialized = true;
}

void RendererGL2::drawMesh(
    const Mesh* mesh,
    const Mat4& matrix,
    const Mat3& texMatrix,
    const Color& color,
    const Texture* texture) const
{
    if( !initialized )
    {
        g2cerror( "RendererGL2 draw called before initialization.\n" );
        exit(0);
    }

    if( !mesh )
        mesh = quad;

    if( mesh == quad )
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, polygonBuffer);
        glBufferData(GL_ARRAY_BUFFER,
            mesh->numberOfVertices()*3*sizeof(float), &(mesh->positions[0]), GL_STREAM_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygonIndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            mesh->numberOfElements()*3*sizeof(unsigned short), &(mesh->indices[0]), GL_STREAM_DRAW);
    }

    glUseProgram(program);

    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 0, 0);

    Mat4 M = projection * matrix;

    for(int i = 0; i < 16; i++)
        fv[i] = (GLfloat)(M.ptr()[i]);
    glUniformMatrix4fv(matrixLocation, 1, false, fv);

    for(int i = 0; i < 9; i++)
        fv[i] = (GLfloat)(texMatrix.ptr()[i]);
    glUniformMatrix3fv(texMatrixLocation, 1, false, fv);

    const GLfloat cf[] = {(GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b, (GLfloat)color.a};
    glUniform4fv(colorLocation, 1, cf);

    if(!texture)
        texture = defaultTexture;

    glActiveTexture(GL_TEXTURE0 + texture->getUnit());
    glBindTexture(GL_TEXTURE_2D, texture->getIndex());
    glUniform1i(textureLocation, texture->getUnit());

    GLuint gltype = GL_TRIANGLES;
    int dimension = 3;

    if( mesh->elementType == Mesh::kLines )
    {
        gltype = GL_LINES;
        dimension = 2;
    }

    glDrawElements(gltype, mesh->numberOfElements()*dimension, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(positionLocation);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
}

Polygon::Polygon() : drawType(kSolid), valid(false), mesh(NULL)
{
    type = "Polygon";
    addProperty("vertices", vertices);
    addProperty("color", color);
    addProperty("matrix", matrix);
}

Polygon::Polygon(const Polygon& P) : drawType(kSolid), valid(false), mesh(NULL)
{
    type = "Polygon";
    this->operator=(P);
}

Polygon::~Polygon()
{
    if( mesh )
        delete mesh;
}

Mat4 Polygon::getMatrix() const
{
    return matrix;
}

Polygon& Polygon::operator=(const Polygon& P)
{
    vertices = P.vertices;
    this->Node::operator=(P);
    this->mesh = NULL;
    this->valid = false;
    return *this;
}

Polygon& Polygon::add( const Vec2& V )
{
    vertices.push_back(V);
    return *this;
}

Polygon& Polygon::add( double x, double y )
{
    vertices.push_back(Vec2(x,y));
    return *this;
}


Polygon& Polygon::operator += (const Vec2& V)
{
    for( VectorProperty<Vec2Property>::iterator itr=vertices.begin(); itr!=vertices.end(); itr++ )
        *itr+=V;
    return *this;
}

Polygon& Polygon::operator -= (const Vec2& V)
{
    for( VectorProperty<Vec2Property>::iterator itr=vertices.begin(); itr!=vertices.end(); itr++ )
        *itr-=V;
    return *this;
}

Polygon& Polygon::operator *= (double k)
{
    for( VectorProperty<Vec2Property>::iterator itr=vertices.begin(); itr!=vertices.end(); itr++ )
        *itr*=k;
    return *this;
}

Polygon& Polygon::operator /= (double k)
{
    for( VectorProperty<Vec2Property>::iterator itr=vertices.begin(); itr!=vertices.end(); itr++ )
        *itr/=k;
    return *this;
}

Polygon Polygon::operator+ (const Vec2& V) const
{
    return Polygon(*this)+=V;
}

Polygon Polygon::operator- (const Vec2& V) const
{
    return Polygon(*this)-=V;
}

Polygon Polygon::operator* (double k) const
{
    return Polygon(*this)*=k;
}

Polygon Polygon::operator/ (double k) const
{
    return Polygon(*this)/=k;
}

Polygon operator * (const Mat4& M, const Polygon& P)
{
    Polygon R;
    R.drawType = P.drawType;

    int n = P.size();

    vector<Vec2> r(n);

    for( int i = 0; i < n; i++)
    {
        Vec4 v = M * Vec4(P.vertices[i], 0.0, 1.0);
        r[i] = (Vec2(v.x, v.y) / v.w);
    }

    R.setVertices(r);
    return R;
}

void Polygon::drawInTree(const Mat4& worldMatrix, const Color& worldColor) const
{
    if( size() < 3 )
        return;

    update();

    if( Mesh::renderer && mesh )
        Mesh::renderer->drawMesh(mesh, worldMatrix, Mat3(), worldColor, NULL);
    else
    {
        g2cerror( "Attempt to draw polygon with no renderer.\n" );
        exit(0);
    }
}

vector<Vec2>::size_type Polygon::size() const
{
    return vertices.size();
}

bool Polygon::empty() const
{
    return vertices.empty();
}

void Polygon::rotate(double theta)
{
    double s = sin(theta), c = cos(theta);
    for(VectorProperty<Vec2Property>::iterator itr = vertices.begin(); itr!= vertices.end(); itr++)
        *itr = Mat2(c, s, -s, c) * (*itr);
}

double angleAToB(const Vec2& A, const Vec2& B)
{
    double det = B.y*A.x-B.x*A.y;
    double sgn = 0.0;
    if(det < 0.0) sgn = -1.0;
    if(det > 0.0) sgn = 1.0;
    return sgn * acos(A.dot(B)/(A.mag()*B.mag()));
}

bool Polygon::vectorInside(const Mat4& worldMatrix, const Vec2& V) const
{
    Vec4 tmp = worldMatrix * Vec4(V.x, V.y, 0.0, 1.0);
    Vec2 tv = Vec2( tmp.x, tmp.y ) / tmp.w;

    double theta = 0;
    int n = size();
    for( int i=0; i<n; i++ )
        theta += angleAToB( vertices[i] - tv, vertices[(i+1)%n] - tv );
    return ( theta > 1 || theta < -1 );
}

void Polygon::update() const
{
    if( valid )
        return;

    if( !mesh )
        mesh = new Mesh;

    int n = size();

    switch(drawType)
    {
        case kSolid:
            mesh->elementType = Mesh::kTriangles;
            mesh->resize(n, max(0, n-2));
        break;

        case kOutline:
            mesh->elementType = Mesh::kLines;
            mesh->resize(n, n);
        break;

        default:
            g2cerror( "drawtype unknown: %d\n", drawType );
            exit(0);
        break;
    }

    int i = 0;
    for( VectorProperty<Vec2Property>::const_iterator itr = vertices.begin(); itr!=vertices.end(); itr++ )
    {
        mesh->positions[i++] = (float)(itr->x);
        mesh->positions[i++] = (float)(itr->y);
        mesh->positions[i++] = 0.0;
    }

    switch( drawType )
    {
        case kSolid:
            triangulate();
        break;

        case kOutline:
            for( int i = 0; i < n; i++ )
            {
                mesh->indices[2*i] = (unsigned short)i;
                mesh->indices[2*i + 1] = (unsigned short)((i+1)%n);
            }
        break;

        default:
            g2cerror( "drawtype unknown: %d\n", drawType );
            exit(0);
        break;
    }

    valid = true;
}

void Polygon::triangulate() const
{
    // This algorithm runs in worst case O(n^3).
    int n = size();
    vector<int> p(n);

    int i = 0, k = 0;

    // Cache the indices of the polygon.
    for( i = 0; i < n; i++ )
        p[i] = i;

    // Iterate through the indices.
    for( i = 0; i < n && n > 2; i++ )
    {
        // Consider triangle i, i+1, i+2 candidate for being an ear.
        const Vec2 &a(vertices[p[i]]), &b(vertices[p[(i+1) % n]]), &c(vertices[p[(i+2) % n]]);
        Mat2 T(b-a, c-a);

        // If it's going clockwise, it's not an ear.
        if( T.det() < 0.0 )
            continue;

        // If another point of the polygon is inside, it's not an ear.
        bool empty = true;
        for( int j = (i+3)%n; j!=i; j=(j+1)%n )
        {
            Vec2 v = T.inverse() * (vertices[p[j]] - a);
            if( v.x > 0.0 && v.y > 0.0 && v.y + v.x < 1.0 )
            {
                empty = false;
                break;
            }
        }

        // If empty is true, at this point, it is an ear, and we remove the
        // middle point from p, and add the triangle to indices.
        if( empty )
        {
            mesh->indices[k++] = (unsigned short)(p[i]);
            mesh->indices[k++] = (unsigned short)(p[(i+1)%n]);
            mesh->indices[k++] = (unsigned short)(p[(i+2)%n]);

            for( int j = (i+1)%n; j < n-1; j++ )
                p[j] = p[j+1];

            i = -1; // Recall i++ happens at the end of the loop-body.
            n--;
        }
    }
}

vector<int> Polygon::getTriangleIndices() const
{
    vector<int> indices;

    // This algorithm runs in worst case O(n^3).
    int n = size();
    vector<int> p(n);

    int i = 0;

    // Cache the indices of the polygon.
    for( i = 0; i < n; i++ )
        p[i] = i;

    // Iterate through the indices.
    for( i = 0; i < n && n > 2; i++ )
    {
        // Consider triangle i, i+1, i+2 candidate for being an ear.
        const Vec2 &a(vertices[p[i]]), &b(vertices[p[(i+1) % n]]), &c(vertices[p[(i+2) % n]]);
        Mat2 T(b-a, c-a);

        // If it's going clockwise, it's not an ear.
        if( T.det() < 0.0 )
            continue;

        // If another point of the polygon is inside, it's not an ear.
        bool empty = true;
        for( int j = (i+3)%n; j!=i; j=(j+1)%n )
        {
            Vec2 v = T.inverse() * (vertices[p[j]] - a);
            if( v.x > 0.0 && v.y > 0.0 && v.y + v.x < 1.0 )
            {
                empty = false;
                break;
            }
        }

        // If empty is true, at this point, it is an ear, and we remove the
        // middle point from p, and add the triangle to indices.
        if( empty )
        {
            indices.push_back( p[i] );
            indices.push_back( p[(i+1)%n] );
            indices.push_back( p[(i+2)%n] );

            for( int j = (i+1)%n; j < n-1; j++ )
                p[j] = p[j+1];

            i = -1; // Recall i++ happens at the end of the loop-body.
            n--;
        }
    }

    return indices;
}

void Polygon::handleChild(const parse::Node* n)
{
    Node::handleChild(n);

    const parse::Node* value = n->data.value;
    string n_name = n->data.s;

    if(n_name == "vertices")
    {
        for(vector<parse::Node*>::const_iterator itr = value->children.begin();
            itr!=value->children.end();
            itr++)
        {
            vertices.push_back(Vec2((*itr)->children[0]->data.x, (*itr)->children[1]->data.x));
        }
    }
}

string Polygon::serializeElements(string indent) const
{
    string r = Node::serializeElements(indent);
    return r;
}

void Polygon::reverse()
{
    vector<Vec2> r;

    int n = size();
    for( int i = 0; i < n; i++ )
        r.push_back(vertices[n-i-1]);

    setVertices(r);
}

void Polygon::setVertices(const vector<Vec2>& v)
{
    vertices.clear();
    for( vector<Vec2>::const_iterator itr = v.begin(); itr != v.end(); itr++ )
    {
        vertices.push_back(*itr);
    }
    valid = false;
}

vector<Vec2> Polygon::getVertices() const
{
    vector<Vec2> r;

    for(VectorProperty<Vec2Property>::const_iterator itr = vertices.begin();
        itr != vertices.end();
        itr++ )
    {
        r.push_back(*itr);
    }

    return r;
}

void Polygon::setDrawType(DrawType inDrawType)
{
    drawType = inDrawType;
    valid = false;
}

Polygon::DrawType Polygon::getDrawType() const
{
    return drawType;
}

Actor::Actor()
    : sampler(NULL)
    , mesh(NULL)
    , sprite(sampler)
    , x(position.x)
    , y(position.y)
{
    init();
}

Actor::Actor(Sampler* insampler)
    : mesh(NULL)
    , sprite(sampler)
    , x(position.x)
    , y(position.y)
{
    sampler = insampler;
    init();
}

Actor::~Actor()
{

}

void Actor::init()
{
    frame = 0;
    k = 1.0;
    visible = true;
    listening = true;

    type = "Actor";

    addProperty("position", position);
    addProperty("k", k);
    addProperty("rotation", rotation);
    addProperty("frame", frame);
    addProperty("color", color);
    addProperty("samplerName", samplerName);
}

Mat4 Actor::getMatrix() const
{
    double c = k*cos(rotation);
    double s = k*sin(rotation);

    return Mat4( c, s, 0, 0,
                -s, c, 0, 0,
                 0, 0, 1, 0,
                 x, y, 0, 1);
}

void Actor::removeSampler(const Sampler* s)
{
    if( sampler == s )
        sampler = NULL;

    Node::removeSampler(s);
}

void Actor::drawInTree(const Mat4& worldMatrix, const Color& worldColor) const
{
    Mat4 matrix(worldMatrix);
    Mat3 texMatrix;

    if( sampler )
    {
        matrix = matrix * sampler->getOffsetMatrix(frame(), 0.0, 0.0, 1.0);
        texMatrix = sampler->getTexMatrix(frame());
    }

    if( !Mesh::renderer )
    {
        g2cerror( "Attempt to draw Actor with no renderer.  Actor: %s\n", name.c_str() );
        exit(0);
    }

    Mesh::renderer->drawMesh(mesh, matrix, texMatrix, worldColor, sampler);

    if( Sprite::drawLines )
        collisionPolygon().drawInTree(worldMatrix, worldColor);
}


static map<Actor*, string> gActorToSamplerName;
static map<Text*, string> gTextToFontName;

void Actor::handleChild(const parse::Node* n)
{
    Node::handleChild(n);

    string n_name = n->data.s;
    parse::Node* value = n->data.value;

    if(value)
    {
        if(n_name == "samplerName")
            gActorToSamplerName[this] = value->data.s;
    }
}

Polygon Actor::collisionPolygon() const
{
    Polygon R;

    if( !polygon.empty() )
    {
        R = k() * polygon + position;
    }
    else if( sampler )
    {
        double w = k() * sampler->getWidth(frame),
               h = k() * sampler->getHeight(frame);

        Polygon samplerPoly( sampler->getCollisionPolygon(frame) );

        if( !samplerPoly.empty() )
        {
            R = k() * samplerPoly + position;
            if( sampler->getCenter() )
                R -= 0.5 * Vec2(w, h);
        }
        else
        {
            double x0 = position.x, x1 = position.x+w, y0 = position.y, y1 = position.y+h;

            R.add(x0, y0).add(x1, y0).add(x1, y1).add(x0, y1);

            if( sampler->getCenter() )
                R -= 0.5 * Vec2(w, h);
        }
    }

    R.setDrawType(Polygon::kOutline);
    return R;
}

bool Actor::vectorInside(const Mat4& worldMatrix, const Vec2& C) const
{
    return collisionPolygon().vectorInside(worldMatrix, Vec2(C.x, C.y));
}

Actor* Actor::actorInClick(const Mat4& worldMatrix, const Vec2& C)
{
    if(!visible())
        return NULL;

    if( vectorInside(worldMatrix * getMatrix(), C) )
        return this;
    return NULL;
}

Button::Button()
    : baseFrame(0)
    , depressed(false)
    , handler(NULL)
    , enabled(true)
{
    type = "Button";
}

Button::Button(Sampler* insampler)
    : Actor(insampler)
    , baseFrame(0)
    , depressed(false)
    , handler(NULL)
    , enabled(true)
{
    type = "Button";
    frame = 0;
}

Button::Button(Sampler* insampler, int baseFrame)
    : Actor(insampler)
    , baseFrame(baseFrame)
    , depressed(false)
    , handler(NULL)
    , enabled(true)
{
    type = "Button";
    frame = baseFrame;
}

void Button::drawInTree(const Mat4& worldMatrix, const Color& worldColor) const
{
    if( !sampler )
    {
        g2cerror( "Button %s drawing with null sampler.\n", name.c_str() );
        exit(0);
    }

    Actor::drawInTree(worldMatrix, worldColor);

    if( Sprite::drawLines )
        collisionPolygon().drawInTree(worldMatrix, worldColor);
}

bool Button::mouseDown(const Mat4& worldMatrix, const Vec2& C)
{
    if( !visible() || !enabled )
        return false;

    frame = baseFrame;

    if( vectorInside(worldMatrix, C) )
    {
        frame = frame() + 1;
        depressed = true;
        if( handler )
            handler->down(this);
        return true;
    }
    return false;
}

void Button::mouseDragged(const Mat4& worldMatrix, const Vec2& C)
{
    if( !visible() || !enabled )
        return;

    frame = baseFrame;
    if( vectorInside(worldMatrix, C) )
    {
        frame = frame() + 1;
    }
    else
    {
        if( depressed )
        {
            depressed = false;
            if( handler )
                handler->up(this);
        }
    }
}

void Button::mouseUp(const Mat4& worldMatrix, const Vec2& C)
{
    if( !visible() || !enabled )
        return;

    if( vectorInside(worldMatrix, C) )
    {
        if( handler )
            handler->click(this);
        else
            g2clog( "WARNING: button %s with no handler.\n", name.c_str() );
    }

    if( depressed )
    {
        depressed = false;
        if( handler )
            handler->up(this);
    }

    frame = baseFrame;
}

void Button::handleChild(const parse::Node* n)
{
    Actor::handleChild(n);

    string n_name = n->data.s;
    parse::Node* value = n->data.value;

    if(value)
    {
        if(n_name == "baseFrame")
            baseFrame = value->data.i;
    }
}

string Button::serializeElements(string indent) const
{
    string r = Actor::serializeElements(indent);
    if( baseFrame )
        r += TAB + indent + "'baseFrame' : " + intToString(baseFrame) + ",\n";
    return r;
}

ButtonHandler::ButtonHandler() {}
ButtonHandler::~ButtonHandler() {}
void ButtonHandler::down(Button* /*button*/) {}
void ButtonHandler::up(Button* /*button*/) {}


Text::Text() : font(NULL),
               justification("left"),
               drawPipe(false)
{
    type = "Text";
}

Text::Text(Font* infont) : Actor(infont),
                           font(infont),
                           justification("left"),
                           drawPipe(false)
{
    type = "Text";
}

void Text::drawInTree(const Mat4& worldMatrix, const Color& worldColor) const
{
    if( font )
    {
        string news = s+(drawPipe?"|":"");
        font->drawString(worldMatrix, worldColor,
            position.x, position.y, k(),
            news.c_str(), justification);
        if( Sprite::drawLines )
            collisionPolygon().drawInTree(worldMatrix, worldColor);
    }
    else
    {
        g2cerror( "Attempt to draw string with no font.\n" );
        g2cerror( "string = %s fontName = %s\n", s.c_str(), fontName.c_str() );
        exit(0);
    }
}

string Text::serializeElements(string indent) const
{
    string r = Actor::serializeElements(indent);

    if( font )
        r += TAB + indent + "'fontName' : " + string("'") + font->name + "',\n";
    if( s != "" )
        r += TAB + indent + "'s' : '" + s + "',\n";
    if( justification != "left" )
        r += TAB + indent + "'justification' : '" + justification + "',\n";
    return r;
}

void Text::handleChild(const parse::Node* n)
{
    Actor::handleChild(n);

    string n_name = n->data.s;
    parse::Node* value = n->data.value;

    if(value)
    {
        if(n_name == "fontName")
        {
            fontName = value->data.s;
            gTextToFontName[this] = value->data.s;
        }

        if(n_name == "s")
            s = value->data.s;

        if(n_name == "justification")
            justification = value->data.s;
    }
}

Polygon Text::collisionPolygon() const
{
    Polygon R;

    if( !polygon.empty() )
    {
        R = polygon + position;
    }
    if( font )
    {
        R = (font->stringRectangle(k, s.c_str(), justification) + position);
    }
    else
    {
        g2cerror( "Attempt to compute string rectangle with no font.\n" );
        exit(0);
    }

    R.setDrawType(Polygon::kOutline);
    return R;
}

void Text::keyboard(unsigned char inkey)
{
    if( delegate )
        delegate->keyboard(inkey);
}

Integer::Integer() : ptr(NULL)
{
    type = "Integer";
}

Integer::Integer(Font* infont, int* inptr) : Text(infont),
                                             ptr(inptr)
{
    type = "Integer";
}

void Integer::drawInTree(const Mat4& worldMatrix, const Color& worldColor) const
{
    if( ptr )
    {
        font->drawString(worldMatrix, worldColor, position.x, position.y, k,
            intToStringWithCommas(*ptr).c_str(), justification);
    }
    else
        Text::drawInTree(worldMatrix, worldColor);
}


Layer::Layer()
{
    type = "Layer";
    addProperty("matrix", matrix);
    addProperty("color", color);
}

Mat4 Layer::getMatrix() const
{
    return matrix;
}

World::World() : bank(NULL), soundInitted(false)
{
    player = new AudioPlayer();
    context = new Context(player);
    context->makeCurrent();
    destroySoundQueue();

    type = "World";

    addProperty("samplers", samplers);
    addProperty("sounds", sounds);
}

World::~World()
{
    if( !soundInitted )
        delete player;

    delete context;

    for(vector<Sampler*>::iterator itr = samplers.begin(); itr!=samplers.end(); itr++)
        delete *itr;

    for(vector<Sound*>::iterator itr = sounds.begin(); itr!=sounds.end(); itr++)
        delete *itr;
}

void World::initSound(AudioPlayer* inAudioPlayer)
{
    if( soundInitted )
    {
        g2cerror("Sound intted for world twice");
        exit(0);
    }

    // Throw away the stub player/context.
    delete context;
    delete player;

    player = inAudioPlayer;
    context = new Context(player);
    context->makeCurrent();

    soundInitted = true; // Only allow once.
}

Actor* World::actorInClick(const Vec2& V)
{
    return Node::actorInClick(getMatrix(), V);
}

void World::drawInTree(const Mat4& worldMatrix, const Color& worldColor) const
{
    if( !Mesh::renderer )
    {
        g2cerror( "Attempt to draw World with no renderer. World: %s\n", name.c_str() );
        exit(0);
    }
    Node::drawInTree(worldMatrix * getMatrix(), worldColor * getColor());
}

void World::resize(int width, int height)
{
    if( !Mesh::renderer )
    {
        g2cerror( "Attempt to resize with no renderer world: %s\n", name.c_str() );
        exit(0);
    }

    Mesh::renderer->projection = Mat4(
        2.0 / width, 0.0, 0.0, 0.0,
        0.0, 2.0 / height, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -1.0, -1.0, 0.0, 1.0);
}

void World::addSampler(Sampler* sampler)
{
    samplers.push_back(sampler);
}

void World::removeSampler(const Sampler* sampler)
{
    int n = samplers.size();
    int j = 0;
    for(int i = 0; i < n; i++)
        if(samplers[i] != sampler)
        {
            samplers[j] = samplers[i];
            j++;
        }
    samplers.resize(j);

    Node::removeSampler(sampler);
}

bool World::mouseDown(const Vec2& C)
{
    clearTookMouseDown();
    return Node::mouseDown(getMatrix(), C);
}

void World::initWithPath(const char* path)
{
    if(bank)
        bank->initSerializableWithPath(this, path);
    else
    {
        g2cerror( "World initiailzied with no bank.\n" );
        exit(0);
    }
}

void World::initWithParseNode(const parse::Node* n)
{
    gActorToSamplerName.clear();
    gTextToFontName.clear();

    Node::initWithParseNode(n);

    updateSamplerMap();
    updateSoundMap();
    updateNodeMap();

    for(map<Actor*, string>::iterator itr = gActorToSamplerName.begin();
        itr != gActorToSamplerName.end();
        itr++)
    {
        const string& samplerName(itr->second);
        if( samplerName != "" )
        {
            map<string, Sampler*>::iterator mitr = samplerMap.find(samplerName);
            if( mitr != samplerMap.end() )
                itr->first->sampler = mitr->second;
            else
            {
                g2cerror( "Actor %s requested sampler not found: %s\n",
                    itr->first->name.c_str(), samplerName.c_str() );
                exit(0);
            }
        }
    }

    for(map<Text*, string>::iterator itr = gTextToFontName.begin();
        itr != gTextToFontName.end();
        itr++)
    {
        const string& fontName(itr->second);
        if( fontName != "" )
        {
            map<string, Sampler*>::iterator mitr = samplerMap.find(fontName);
            if( mitr != samplerMap.end() )
                itr->first->font = (Font*)(mitr->second);
        }
    }
}

void World::updateSamplerMap()
{
    samplerMap.clear();
    for(vector<Sampler*>::iterator itr = samplers.begin(); itr!=samplers.end(); itr++)
    {
        Sampler* sampler = *itr;
        samplerMap[sampler->name] = sampler;
    }
}

void World::updateSoundMap()
{
    soundMap.clear();
    for(vector<Sound*>::iterator itr = sounds.begin(); itr!=sounds.end(); itr++)
    {
        Sound* sound = *itr;
        soundMap[sound->name] = sound;
    }
}

void World::updateNodeMap(Node* node)
{
    if( !node )
        node = this;

    nodeMap[node->name] = node;

    for(vector<Node*>::iterator itr = node->children.begin(); itr!=node->children.end(); itr++)
        updateNodeMap(*itr);
}


void World::handleChild(const parse::Node* n)
{
    string n_name = n->data.s;
    parse::Node* value = n->data.value;

    bool handled = false;

    if( value )
    {
        if( n_name == "samplers" )
        {
            for(vector<parse::Node*>::const_iterator itr = value->children.begin();
                itr!=value->children.end();
                itr++)
            {
                string type = getType(*itr);
                Sampler* sampler = NULL;

                if(type=="Sprite")
                {
                    sampler = new Sprite();
                }
                else if(type == "Font")
                {
                    sampler = new Font();
                }
                else
                {
                    g2cerror("Object in sampler list must be type Sprite or Font.\n");
                    exit(0);
                }

                if( sampler )
                {
                    sampler->initWithParseNode(*itr);

                    samplers.push_back(sampler);
                    deleteResources.push_back(sampler);

                    bank->initTextureWithPath(sampler, sampler->file().c_str());
                }
            }

            handled = true;
        }

        if( n_name == "sounds" )
        {
            for(vector<parse::Node*>::const_iterator itr = value->children.begin();
                itr!=value->children.end();
                itr++)
            {
                string type = getType(*itr);
                Sound* sound = NULL;

                if(type=="Sound")
                    sound = new Sound();
                else
                {
                    g2cerror("Object in sound list must be type Sound.\n");
                    exit(0);
                }

                if( sound )
                {
                    sound->initWithParseNode(*itr);
                    sounds.push_back(sound);
                    deleteResources.push_back(sound);
                    bank->initSoundWithPath(sound, sound->file.c_str());
                }
            }

            handled = true;
        }
    }

    if( !handled )
    {
        Layer::handleChild(n);
    }
}

void World::clear()
{
    clearChildren();

    samplers.clear();
    sounds.clear();

    for( vector<Serializable*>::iterator itr = deleteResources.begin();
         itr != deleteResources.end();
         itr++ )
    {
        delete *itr;
    }

    deleteResources.clear();
}


int gTimes = 0;
std::string gLastName;


void World::playSound(const std::string& name, double gain) const
{
    if( sources.empty() )
        initSoundQueue();

    map<string, Sound*>::const_iterator sound_itr = soundMap.find(name);
    if( sound_itr!=soundMap.end() )
    {
        Sound* sound = sound_itr->second;
        for(vector<Source*>::iterator itr = sources.begin(); itr!=sources.end(); itr++)
        {
            Source* source = *itr;
            if( !source->isPlaying() )
            {
                sound->source = source;
                sound->play(gain);
                break;
            }
        }
    }
}

Sound* World::getSound(const string& name)
{
    map<string, Sound*>::iterator itr = soundMap.find(name);
    if( itr != soundMap.end() )
        return itr->second;
    return NULL;
}

Node* World::getNode(const string& name)
{
    map<string, Node*>::iterator itr = nodeMap.find(name);
    if( itr != nodeMap.end() )
        return itr->second;
    return NULL;
}

Sampler* World::getSampler(const string& name)
{
    map<string, Sampler*>::iterator itr = samplerMap.find(name);
    if( itr != samplerMap.end() )
        return itr->second;
    return NULL;
}


void World::initSoundQueue() const
{
    context->makeCurrent();
    sources.resize(8);

    for(vector<Source*>::iterator itr = sources.begin(); itr!=sources.end(); itr++)
        *itr = new Source;
}

void World::destroySoundQueue() const
{
    for(vector<Source*>::iterator itr = sources.begin(); itr!=sources.end(); itr++)
        delete *itr;
}


Animator::Animator() : eventStoppedCounter(0)
{
    animationsAdded = 0;
    animationsRemoved = 0;
    animationsRemovedNotInList = 0;
}

Animator::~Animator()
{

}

void Animator::add(Animation* a)
{
    animationsAdded++;

    retain(a);
    S.insert(a);

    if(a->stopsEvents)
    {
        if( eventStoppedCounter==0 )
        {
            for(vector<Listener*>::iterator itr = listeners.begin();
                itr!=listeners.end();
                itr++)
            {
                (*itr)->listening = false;
            }
        }
        eventStoppedCounter++;
    }
}

void Animator::retain(Animation* a)
{
    a->referenceCounter++;
}

bool Animator::release(Animation* a)
{
    assert(a->referenceCounter > 0);
    a->referenceCounter--;
    if( a->referenceCounter <= 0 )
    {
        delete a;
        return true;
    }
    return false;
}

void Animator::remove(Animation* a)
{
    set<Animation*>::iterator itr = S.find(a);
    if(itr!=S.end())
    {
        animationsRemoved++;

        if(a->stopsEvents)
        {
            eventStoppedCounter--;
            if(eventStoppedCounter<=0)
            {
                for(vector<Listener*>::iterator itr = listeners.begin();
                    itr!=listeners.end();
                    itr++)
                {
                    (*itr)->listening = true;
                }
            }

            if(eventStoppedCounter < 0)
            {
                g2cerror( "Animator event stopped counter dropped below 0.\n" );
                exit(0);
            }
        }
        S.erase(itr);
        release(a);
    }
    else
    {
        animationsRemovedNotInList++;
    }
}

class AnimationContainer
{
public:
    double t;
    int i;
    Animation* a;

    AnimationContainer(double t, int i, Animation* a) : t(t), i(i), a(a) {}

    bool operator<(const AnimationContainer& c) const
    {
        return t < c.t || (t == c.t && i < c.i);
    }
};

void Animator::step(double t)
{
    set<AnimationContainer> beginMe, advanceMe, endMe;

    int index = 0;
    for(set<Animation*>::iterator itr = S.begin(); itr!= S.end(); itr++)
    {
        Animation* a = *itr;

        if(t > a->start)
        {
            if(!a->running)
                beginMe.insert(AnimationContainer(a->start, index, a));

            advanceMe.insert(AnimationContainer(a->start, index, a));
        }

        if(!a->forever && t > a->start + a->duration)
            endMe.insert(AnimationContainer(a->start + a->duration, index, a));

        index++;
    }

    for(set<AnimationContainer>::iterator itr = beginMe.begin(); itr!= beginMe.end(); itr++)
    {
        Animation* a = itr->a;
        a->begin();
        a->running = true;
    }

    for(set<AnimationContainer>::iterator itr = advanceMe.begin(); itr!= advanceMe.end(); itr++)
    {
        Animation* a = itr->a;
        a->advance(t);
        a->last = t;
    }

    for(set<AnimationContainer>::iterator itr = endMe.begin(); itr!= endMe.end(); itr++)
    {
        Animation* a = itr->a;
        a->end();
        remove(a);
    }
}

void Animator::end()
{
    vector<Animation*> endMe;

    for(set<Animation*>::iterator itr = S.begin(); itr!= S.end(); itr++)
    {
        endMe.push_back(*itr);
    }

    for(vector<Animation*>::iterator itr = endMe.begin(); itr!= endMe.end(); itr++)
    {
        Animation* a = *itr;

        a->end();
        remove(a);
    }
}

void Animator::clear()
{
    vector<Animation*> deleteMe;

    for(set<Animation*>::iterator itr = S.begin(); itr!= S.end(); itr++)
    {
        Animation* a = *itr;
        deleteMe.push_back(a);
    }

    for(vector<Animation*>::iterator itr = deleteMe.begin(); itr!= deleteMe.end(); itr++)
    {
        Animation* a = *itr;
        remove(a);
    }

    assert(S.size() == 0);
}

Animation::Animation(double instart) : stopsEvents(false),
                                       running(false),
                                       forever(true),
                                       duration(0.0),
                                       referenceCounter(0)
{
    last = start = currentTime() + instart;
}

Animation::Animation(double instart, double induration) : stopsEvents(false),
                                                          running(false),
                                                          forever(false),
                                                          referenceCounter(0)
{
    last = start = currentTime() + instart;
    duration = induration;
}

Animation::~Animation()
{

}

void Animation::advance(double t)
{
    double scaled_t = 0.0;
    if( duration )
    {
        scaled_t = (t - start) / (duration);
        if( scaled_t < 0.0 )
            scaled_t = 0.0;
        if( scaled_t > 1.0 )
            scaled_t = 1.0;
    }

    step(scaled_t);
}

void Animation::begin()
{

}

void Animation::step(double /*t*/)
{

}

void Animation::end()
{

}

void Animation::print() const
{
    g2clog( "start: %f duration: %f stopsEvents: %d", start, duration, stopsEvents );
}

void Animation::display() const
{
    print();
    g2clog( "\n" );
}

} // end namespace


