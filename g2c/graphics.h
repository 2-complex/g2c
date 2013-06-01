
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
#include "transforms.h"
#include "element.h"
#include "texture.h"


namespace g2c {




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

} // end namespace


#endif
