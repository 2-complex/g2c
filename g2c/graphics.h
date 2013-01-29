
#ifndef _GRAPHICS_
#define _GRAPHICS_

#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>

#include "opengl.h"

#include "bank.h"
#include "util.h"
#include "lin/lin.h"
#include "element.h"
#include "texture.h"


class Model;
class Effect;
class Assumption;
class Geometry;
class Shape;
class Buffer;
class IndexBuffer;
class Field;


class Value {
public:
	Value();
	Value(const Value& v);
	~Value();
	
	Value(float t);
	Value(const Vec2& t);
	Value(const Vec3& t);
	Value(const Vec4& t);
	Value(const Mat2& t);
	Value(const Mat3& t);
	Value(const Mat4& t);
	Value(const Texture* t);
	
	Value& operator=(const Value& v);
	
	enum Type {
		NONE,
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		MAT2,
		MAT3,
		MAT4,
		TEXTURE
	} type;
	
	float getFloat() const;
	Vec2 getVec2() const;
	Vec3 getVec3() const;
	Vec4 getVec4() const;
	Mat2 getMat2() const;
	Mat3 getMat3() const;
	Mat4 getMat4() const;
	const Texture* getTexture() const;
	
	void applyToLocation(GLuint location) const;
	std::string toString() const;
	
	std::string textureName;
private:
	union Data {
		GLfloat* ptr;
		const Texture* texture;
	} data;
	
	int size;
};

class Effect : public Element {
friend class Geometry;
friend class Shape;
public:
	Effect();
	virtual ~Effect();
	
	std::string vertexCode;
	std::string fragmentCode;
	
	void compile();
	
protected:
	virtual std::string serializeElements(std::string indent = "") const;
	virtual void handleChild(const parse::Node* n);

private:
	void assume(const std::map<std::string, Value>* assumption) const;
	void use() const;
	
	void bindAttributeToField(const std::string& name, const Field& field) const;
	void disableEnabledAttributes() const;
	
	bool loadShaders();
	void loadVertexShader(const char* s);
	void loadFragmentShader(const char* s);
	bool compileShader(GLuint *shader, GLenum type, const char* code);
	bool linkProgram();
	
	void addAttributesFromProgram();
	void addUniformsFromProgram();
	
	void addAttribute(const char* name);
	void addUniform(const char* name);

	mutable std::vector<GLint> enabledAttributes;
	
	std::map<std::string, GLint> attributeLocationMap;
	std::map<std::string, GLint> uniformLocationMap;
	
	GLuint program;
	GLuint vertShader;
	GLuint fragShader;
};

class Assumption : public std::map<std::string, Value>,
				   public Element {
public:
	Assumption();
	
	bool active;
	std::string effectName;
	Effect* effect;
	
protected:
	virtual std::string serializeElements(std::string indent = "") const;
	virtual void handleChild(const parse::Node* n);
};

class Buffer : public Element {
friend class Effect;
public:
	Buffer();
	Buffer(const std::vector<double>& v);
	Buffer(const std::vector<float>& v);
	Buffer(const double* v, int size);
	Buffer(const float* v, int size);
	virtual ~Buffer();

protected:
	virtual std::string serializeElements(std::string indent = "") const;
	virtual void handleChild(const parse::Node* n);

private:
	void initWithFloatArray(const float* v);
	GLuint size;
	GLuint index;
};


class IndexBuffer : public Element {
friend class Geometry;
public:
	IndexBuffer();
	IndexBuffer(const std::vector<int>& v);
	IndexBuffer(const std::vector<unsigned short>& v);
	IndexBuffer(const int* v, int size);
	IndexBuffer(const unsigned short* v, int size);
	virtual ~IndexBuffer();
	
protected:
	virtual std::string serializeElements(std::string indent = "") const;
	virtual void handleChild(const parse::Node* n);

private:
	void initWithShortArray(const unsigned short* v);
	GLuint size;
	GLuint index;
};


class Field : public Element {
friend class Effect;
public:
	Field();
	Field(const Buffer* buffer, GLint size, GLsizei stride=0, int offset=0);
	virtual ~Field();
	
	std::string bufferName;
	const Buffer* buffer;
	
protected:
	virtual std::string serializeElements(std::string indent = "") const;
	virtual void handleChild(const parse::Node* n);
	
private:
	GLint size;
	GLsizei stride;
	int offset;
};


class Geometry : public std::map<std::string, Field>,
				 public Element {
friend class Shape;
public:
	Geometry();
	virtual ~Geometry();
	
	std::string indicesName;
	const IndexBuffer* indices;
	void draw() const;

protected:
	virtual std::string serializeElements(std::string indent = "") const;
	virtual void handleChild(const parse::Node* n);
	
private:
	void bindAttributes(const Effect* effect);
};


class Shape : public Element {
public:
	Shape();
	
	bool visible;
	
	std::string geometryName;
	std::vector<std::string> assumptionNames;
	
	Geometry* geometry;
	std::list<Assumption*> assumptions;
	
	void draw() const;
	const Value& get(const std::string& uniformName) const;

protected:
	virtual std::string serializeElements(std::string indent = "") const;
	virtual void handleChild(const parse::Node* n);
};


class Model : public Serializable {
friend class Element;
friend class Texture2D;
friend class CubeMap;
friend class Effect;
friend class Assumption;
friend class Geometry;
friend class Shape;
friend class Buffer;
friend class IndexBuffer;
friend class Field;

public:
	Model();
	virtual ~Model();
	
	Bank* bank;
	
	void draw() const;
	
	Model& add(Element* element, bool flagForDelete = false);
	Model& add(const Model& element, bool flagForDelete = false);
	
	void makeMaps();
	void compileEffects();
	void resolveNames();
	
	std::vector<std::string> include;
	
	std::map<std::string, Texture*> textures;
	std::map<std::string, Assumption*> assumptions;
	std::map<std::string, Effect*> effects;
	std::map<std::string, Buffer*> buffers;
	std::map<std::string, IndexBuffer*> indexBuffers;
	std::map<std::string, Geometry*> geometries;
	std::map<std::string, Shape*> shapes;
	
protected:
	std::set<Element*> deleteMe;
	std::vector<Element*> elements;
	
	virtual std::string serializeElements(std::string indent = "") const;
	virtual void deserialize(const std::string& s);
	virtual void handleChild(const parse::Node* n);
};


/*	Computes a perspective transformation matrix given the angular height
	of the frustum, the aspect ratio, and the near and far clipping planes.*/
Mat4 perspective(double angle, double aspect, double near, double far);

/*	Computes an orthographic projection matrix given the coordinates of the
	planes defining the viewing volume.*/
Mat4 orthographic(double left, double right, double bottom, double top, double near, double far);

/*	Computes a perspective transformation matrix given the left, right,
	top, bottom, near and far clipping planes.*/
Mat4 frustum(double left, double right, double bottom, double top, double near, double far);

/*	Computes a look-at transformation.*/
Mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up);

/*	Creates a matrix which translates by the given vector v.*/
Mat4 translate(const Vec3& v);

/*	Creates a matrix which scales in each dimension by an amount given by
	the corresponding entry in the given vector.*/
Mat4 scale(const Vec3& v);

/*	Creates a matrix which rotates around the x-axis by the given angle.*/
Mat4 rotateX(double theta);

/*	Creates a matrix which rotates around the y-axis by the given angle.*/
Mat4 rotateY(double theta);

/*	Creates a matrix which rotates around the z-axis by the given angle.*/
Mat4 rotateZ(double theta);

/*	Creates a matrix which rotates around the given axis by the given.*/
Mat4 axisRotate(const Vec3& axis, double theta);

/*	Retrieves the upper 3x3 matrix of a 4x4 transformation matrix.*/
Mat3 upper3x3(const Mat4& m);


#endif
