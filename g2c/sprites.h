
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

#include <vector>
#include <string>
#include <set>
#include <map>

#include "opengl.h"

#include "util.h"
#include "lin/lin.h"

#include "listener.h"
#include "parse.h"
#include "serializable.h"
#include "bank.h"
#include "texture.h"

#if !defined(STUB_SOUND)
#include "sound.h"
#endif


namespace g2c {
    class Color;
    class Node;
    class Animator;
    class Animation;
    class Renderer;
    class Sprite;
    class Button;
    class ButtonHandler;
    class Font;
    class Actor;
    class Layer;
    class World;
    class Mesh;
    class Polygon;
    
    
    class Mat4Property : public Mat4,
                         public Serializable {
    public:
        Mat4Property();
        Mat4Property(const Mat4& m);
        
        virtual std::string serialize(std::string indent) const;
        void initWithParseNode(const parse::Node* n);
    };

    class Vec2Property : public Vec2,
                         public Serializable {
    public:
        Vec2Property();
        Vec2Property(const Vec2& v);
        
        virtual std::string serialize(std::string indent) const;
        void initWithParseNode(const parse::Node* n);
    };
    
    class Vec3Property : public Vec3,
                         public Serializable {
    public:
        Vec3Property();
        Vec3Property(const Vec3& v);
        
        virtual std::string serialize(std::string indent) const;
        void initWithParseNode(const parse::Node* n);
    };
    
    class Color : public Vec4 {
    public:
        Color() : Vec4(), r(x), g(y), b(z), a(w) { r = g = b = a = 1.0; }
        Color(double r, double g, double b, double a) : Vec4(r, g, b, a), r(x), g(y), b(z), a(w) {}
        Color(const Color& c) : Vec4(c), r(x), g(y), b(z), a(w) {}
        Color(const Vec4& c) : Vec4(c), r(x), g(y), b(z), a(w) {}
        inline Color& operator= (const Color& v) {x=v.x; y=v.y; z=v.z; w=v.w; return *this;}
        
        double &r, &g, &b, &a;
    };
    
    class ColorProperty : public Color,
                          public Serializable {
    public:
        ColorProperty();
        ColorProperty(const Color& c);
        
        inline ColorProperty& operator= (const Color& v) {x=v.x; y=v.y; z=v.z; w=v.w; return *this;}
        
        virtual std::string serialize(std::string indent) const;
        void initWithParseNode(const parse::Node* n);
    };
    
    
    class Node : public Serializable,
                 public Listener {
    public:
        Node();
        virtual ~Node();
        
        BoolProperty visible;
        
        Mat4Property matrix;
        ColorProperty color;
        
        Node* parent;
        PointerVectorProperty<Node*> children;
        
        Node* findChild(const std::string& name) const;
        void add(Node* t);
        void addAtIndex(Node* t, int index);
        int indexOf(const Node* t) const;
        
        void remove(Node* t);
        void removeAndDelete(Node* t);
        virtual void removeSprite(const Sprite* sprite);
        
        void clearChildren();
        
        virtual void draw() const;
        virtual Actor* actorInClick(const Vec2& C);
        
        virtual void handleChild(const parse::Node* n);
        
        virtual bool vectorInside( const Vec2& V ) const;
        
        // Methods of Listener.
        virtual void keyboard(unsigned char inkey);
        virtual bool mouseDown(const Vec2& C);    
        virtual void mouseDragged(const Vec2& C);
        virtual void mouseUp(const Vec2& C);
        
        const Mat4& getWorldMatrix() const;
        const Color& getWorldColor() const;
        
    protected:
        mutable Mat4 worldMatrix;
        mutable Color worldColor;
        
        std::vector<Serializable*> deleteMe;
       	
        bool tookMouseDown;
        void clearTookMouseDown();
    };
    
    class Mesh {
    public:
        Mesh();
        virtual ~Mesh();
        
        enum ElementType {
            kTriangles,
            kLines
        };
        
        const float* getPositions() const;
        const short* getIndices() const;
        
        int numberOfVertices;
        int numberOfElements;
        
        void resize(int inNumberOfVertices, int inNumberOfElements);
        
        ElementType elementType;
        
        float* positions;
        short* indices;
    };
    
    class Renderer {
    public:
        Renderer();
        virtual ~Renderer();
        
        Mat4 projection;
    
    protected:
        mutable GLfloat fv[16];
        Mesh* quad;
        
    public:
        virtual void init() = 0;
        virtual void drawMesh(const Mesh* mesh,
                              const Mat4& matrix,
                              const Mat3& texMatrix,
                              const Color& color,
                              const Texture* texture) const = 0;
        
        virtual void drawMesh(const Mesh* m, const Node* n) const;
    };
    
    class RendererGL1 : public Renderer {
    public:
        RendererGL1();
        ~RendererGL1();
        
        virtual void init();
        virtual void drawMesh(const Mesh* mesh,
                              const Mat4& matrix,
                              const Mat3& texMatrix,
                              const Color& color,
                              const Texture* texture) const;
    };
    
    class RendererGL2 : public Renderer {
    public:
        RendererGL2();
        ~RendererGL2();
        
    private:
        GLuint buffer,
               indexBuffer,
               program,
               positionLocation,
               vertexShader,
               fragmentShader,
               matrixLocation,
               SpriteLocation,
               colorLocation,
               textureLocation,
               texMatrixLocation,
               polygonBuffer,
               polygonIndexBuffer;
        
        Texture2D* defaultTexture;
        
    public:
        virtual void init();
        
    protected:
        bool initialized;
        virtual void drawMesh(const Mesh* m,
                              const Mat4& matrix,
                              const Mat3& texMatrix,
                              const Color& color,
                              const Texture* texture) const;
    };

    
    class Polygon : public Node {
    public:
        Polygon();
        virtual ~Polygon();
        
        enum DrawType {
            kSolid,
            kOutline,
        };
        
        Polygon(const Polygon& P);
        Polygon& operator=(const Polygon& P);
        
        Polygon& add( const Vec2& V );
        Polygon& add( double x, double y );
                
        Polygon& operator += (const Vec2& V);
        Polygon& operator -= (const Vec2& V);
        Polygon& operator*= (double k);
        Polygon& operator/= (double k);
        Polygon operator + (const Vec2& V) const;
        Polygon operator - (const Vec2& V) const;
        Polygon operator* (double k) const;
        Polygon operator/ (double k) const;
        friend class Polygon operator * (const Mat4&, const Polygon& P);
        inline friend class Polygon operator * (double k, const Polygon& P) {return P*k;}
        inline friend class Polygon operator + (const Vec2& V, const Polygon& P) {return P+V;}
        void rotate(double theta);
        
        virtual void draw() const;
        
        std::vector<Vec2>::size_type size() const;
        bool empty() const;
        
        bool vectorInside( const Vec2& V ) const;
        
        void reverse();
        void setVertices(const std::vector<Vec2>& p);
        std::vector<Vec2> getVertices() const;
        
        DrawType getDrawType() const;
        void setDrawType(DrawType inDrawType);
                        
        virtual void handleChild(const parse::Node* n);
        virtual std::string serializeElements(std::string indent = "") const;
        
        VectorProperty<Vec2Property> vertices;
        
    private:
        mutable Mesh* mesh;
        DrawType drawType;
        mutable bool valid;
        
        void update() const;
        void triangulate() const;
    };
    
    class Actor : public Node {
    public:
        Actor();
        explicit Actor(Sprite* insprite);
        virtual ~Actor();
        
        Sprite* sprite;
        Mesh* mesh;
        
        double& x;
        double& y;
        DoubleProperty k;
        
        IntProperty frame;
        Vec2Property position;
        
        Polygon polygon;
        
        virtual void draw() const;
        virtual void handleChild(const parse::Node* n);
        virtual Polygon collisionPolygon() const;
        bool vectorInside(const Vec2& C) const;
        virtual Actor* actorInClick(const Vec2& C);
        virtual void removeSprite(const Sprite* sprite);
        
    private:
        void init();
        
        StringProperty spriteName;
    };
    
    class Text : public Actor {
    public:
        Text();
        Text(Font* infont);
        
        std::string justification;
        std::string s;
        std::string fontName;
        Font* font;
        bool drawPipe;
        
        void setString(const std::string& ins);
        virtual void draw() const;
        
        virtual std::string serializeElements(std::string indent) const;
        virtual void handleChild(const parse::Node* n);
        
        virtual Polygon collisionPolygon() const;
        
        // Methods of listener.
        void keyboard(unsigned char inkey);
    };
    
    class Integer : public Text {
    public:
        Integer();
        Integer(Font* infont, int* inptr);
        
        int* ptr;
        
        virtual void draw() const;
    };
    
    class Sprite : public Texture2D {
    public:
        Sprite();
        virtual ~Sprite();
        
        StringProperty file;
        IntProperty numberOfColumns;
        IntProperty numberOfRows;
        
        BoolProperty center;
        BoolProperty flipRows;
        
        Polygon polygon;
        
        static bool drawLines;
        static Renderer* renderer;
        
        Mat4 getOffsetMatrix(double x = 0.0, double y = 0.0, double k = 1.0) const;
        Mat3 getTexMatrix(int frame = 0) const;
        
    protected:
        virtual std::string serializeElements(std::string indent = "") const;
        virtual void handleChild(const parse::Node* n);
    };
    
    class Button : public Actor {
    public:
        Button();
        explicit Button(Sprite* insprite);
        Button(Sprite* insprite, int inBaseFrame);
        
        int baseFrame;
        bool depressed;
        
        virtual void draw() const;
        
        virtual bool mouseDown(const Vec2& C);
        virtual void mouseDragged(const Vec2& C);
        virtual void mouseUp(const Vec2& C);
        
        virtual void handleChild(const parse::Node* n);
        std::string serializeElements(std::string indent) const;
        
        ButtonHandler* handler;
    };
    
    class ButtonHandler {
    public:
        ButtonHandler();
        virtual ~ButtonHandler();
        
        virtual void down(Button* button);
        virtual void up(Button* button);
        
        virtual void click(Button* button) = 0;
    };
    
    class Font : public Sprite {
    public:
        Font();
        
        std::vector<double> widths;
        std::vector<double> lefts;
        double lineHeight;
        double lineBottom;
        double widthScale;
        double spacing;
        char baseChar;
        
        double charWidth(char c) const;
        double charLeft(char c) const;
        double lineWidth(double k, const char* s, int startIndex) const;
        std::vector<std::pair<int, int> > lineIndices(const char* s) const;
        void drawString(const Mat4& M,
                        const Color& color,
                        double x,
                        double y,
                        double k,
                        const char* s,
                        const std::string& justification = "left") const;
        Polygon stringRectangle(double k,
                                const char* s,
                                const std::string& justification = "left") const;
        
        void getWidthsFromBitmap(const Bitmap& bitmap);
        
        virtual std::string serializeElements(std::string indent = "") const;
        virtual void handleChild(const parse::Node* n);
    };
    
    class Layer : public Node {
    public:
        Layer();
        virtual ~Layer() {}
    };
    
    class World : public Node {
        friend class Actor;
    public:
        World();
        virtual ~World();
        
        Bank* bank;
        
        PointerVectorProperty<Sprite*> sprites;
#if !defined(STUB_SOUND)
        PointerVectorProperty<Sound*> sounds;
#endif
        
        virtual void draw() const;
        
        virtual void resize(int width, int height);
        
        void addSprite(Sprite* sprite);
        virtual void removeSprite(const Sprite* sprite);
        
        void clear();
        
        // Methods of Listener.
        virtual void keyboard(unsigned char inkey);
        virtual bool mouseDown(const Vec2& C);    
        virtual void mouseDragged(const Vec2& C);
        virtual void mouseUp(const Vec2& C);
        
        virtual void initWithPath(const char* path);
        virtual void initWithParseNode(const parse::Node* n);
        virtual void handleChild(const parse::Node* n);
        
        std::string serializeSprites(std::string indent = "") const;
        
        virtual void playSound(const std::string& name) const;
        g2c::Sound* getSound(const std::string& name);
        virtual Node* getNode(const std::string& name);
        virtual Sprite* getSprite(const std::string& name);
        
    private:
        std::map<std::string, Sprite*> spriteMap;
#if !defined(STUB_SOUND)
        std::map<std::string, Sound*> soundMap;
#endif
        std::map<std::string, Node*> nodeMap;
        
        std::vector<Serializable*> deleteResources;
        
        void buildMaps();
        void updateSpriteMap();
        void updateSoundMap();
        void updateNodeMap(Node* node = NULL);
        
        // These are const because they get called
        // by playSound (which really should be const).
        void initSoundQueue() const;
        void destroySoundQueue() const;
        
#if !defined(STUB_SOUND)
        mutable Context* context;
        mutable std::vector<Source*> sources;
#endif
    };
    
    class Animation {
    friend class Animator;
    public:
        Animation(double instart);
        Animation(double instart, double induration);
        virtual ~Animation();
        
        bool stopsEvents;
        bool forever;
        bool running;
        
        double last;
        double start;
        double duration;
        
        virtual void advance(double t);
        
        virtual void begin();
        virtual void step(double t);
        virtual void end();
        
        void print() const;
        void display() const;
        
    private:
        int referenceCounter;
    };
    
    class Animator {
    public:
        Animator();
        virtual ~Animator();
        
        int eventStoppedCounter;
        
        std::set<Animation*> S;
        std::vector<Listener*> listeners;
        
        void add(Animation*);
        void remove(Animation*);
        
        void retain(Animation*);
        bool release(Animation*);
        
        void step(double t);
        
        void clear();
        
        int animationsAdded;
        int animationsRemoved;
        int animationsRemovedNotInList;
    };
    
    void setDebugString(std::string* inDebugString);
    
} // end namespace


#endif
