
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

namespace sprite {

bool Sprite::drawLines = false;
Renderer* Sprite::renderer = NULL;

std::string* gDebugString = NULL;


void setDebugString(std::string* inDebugString)
{
	gDebugString = inDebugString;
}

Sprite::Sprite() : numberOfRows(1),
				   numberOfColumns(1),
				   center(false),
				   flipRows(false)
{
	type = "Sprite";
}

Sprite::~Sprite()
{
}

Mat4 Sprite::getOffsetMatrix(double x, double y, double k) const
{
	double w = width / numberOfColumns,
		   h = height / numberOfRows;
	
	Vec2 offset(x, y);
	if( center )
		offset -= 0.5 * k * Vec2(w, h);
	
	return Mat4(
		k*w, 0.0, 0.0, 0.0,
		0.0, k*h, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		offset.x, offset.y, 0.0, 1.0);
}

Mat3 Sprite::getTexMatrix(int frame) const
{
	int c = numberOfColumns;
	int r = numberOfRows;
	
	double i = (frame % c + c) % c,
		   j = ((frame / c) % r + r) % r;
	
	if( flipRows )
		j = numberOfRows - j - 1;
	
	return Mat3(1.0 / c, 0.0, 0.0,
		 		0.0, 1.0 / r, 0.0,
				i / c, j / r, 1.0);
}

void Sprite::handleChild(const parse::Node* n)
{
	string n_name = n->data.s;
	parse::Node* value = n->data.value;
	
	if(n_name == "file")
		file = value->data.s;
	
	if(n_name == "name")
		name = value->data.s;
	
	if(n_name == "numberOfColumns")
		numberOfColumns = value->data.i;
	
	if(n_name == "numberOfRows")
		numberOfRows = value->data.i;
	
	if(n_name == "center")
		center = value->data.i ? true : false;

	if(n_name == "flipRows")
		flipRows = value->data.i ? true : false;
	
	if(n_name == "polygon")
	{
		vector<Vec2> pl;
		for(vector<parse::Node*>::const_iterator itr = value->children.begin();
			itr!=value->children.end();
			itr++)
		{
			pl.push_back(Vec2((*itr)->children[0]->data.x, (*itr)->children[1]->data.x));
		}
		polygon.setVertices(pl);
	}
}

string Sprite::serializeElements(string indent) const
{
	string r = Serializable::serializeElements(indent);
	r += TAB + indent + "'file' : " + string("'") + file + "',\n";
	if( numberOfColumns != 1 )
		r += TAB + indent + "'numberOfColumns' : " + intToString(numberOfColumns) + ",\n";
	if( numberOfRows != 1 )
		r += TAB + indent + "'numberOfRows' : " + intToString(numberOfRows) + ",\n";
	if( center )
		r += TAB + indent + "'center' : " + intToString(center) + ",\n";
	if( flipRows )
		r += TAB + indent + "'flipRows' : " + intToString(flipRows) + ",\n";
	
	if( !polygon.empty() )
	{
		vector<Vec2> vertices = polygon.getVertices();
		r += TAB + indent + "'polygon' : " + "[";
		for(vector<Vec2>::const_iterator itr = vertices.begin(); itr!=vertices.end(); itr++)
			r += "[" + floatToString(itr->x) + ", " + floatToString(itr->y) + "], ";
		r += "],\n";
	}
	
	return r;
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
		printf( "charWidth: Attempt to access off "
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
	for(int i = startIndex; c=s[i]; i++)
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
	
	for(; c=s[i]; i++)
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
			
			Mat4 matrix(M * getOffsetMatrix(x-l, y, k));
			Mat3 texMatrix(getTexMatrix(c-baseChar));
			
			Sprite::renderer->drawMesh(NULL, matrix, texMatrix, color, this);
			
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
		if( i % numberOfColumns == 0 )
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
		if( i % numberOfColumns == 0 )
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


Node::Node() : visible(true), parent(NULL) {type = "Node";}

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

void Node::removeAndDelete(Node* t)
{
	remove(t);
	delete t;
}

void Node::removeSprite(const Sprite* s)
{	
	for(vector<Node*>::const_iterator itr = children.begin(); itr!=children.end(); itr++)
		(*itr)->removeSprite(s);
}

Node* Node::findChild(const string& name) const
{
	for(vector<Node*>::const_iterator itr = children.begin(); itr!=children.end(); itr++)
	{
		if( (*itr)->name == name )
			return *itr;
	}
	printf( "WARNING: child %s of node %s not found.\n", name.c_str(), this->name.c_str() );
	return NULL;
}

void Node::draw() const
{
	for(vector<Node*>::const_iterator itr = children.begin(); itr!=children.end(); itr++)
	{
		Node* node = *itr;
		
		if(node->visible)
		{
			node->worldMatrix = worldMatrix * node->matrix;
			node->worldColor = worldColor * node->color;
			node->draw();
		}
	}
}

string Node::serializeElements(string indent) const
{
	string r = Serializable::serializeElements(indent);
	
	if( matrix != Mat4() )
	{
		string s = "[";
		for(int i = 0; i < 16; i++)
			s += floatToString(matrix.ptr()[i]) + ((i==15) ? "" : ", ");
		s += "]";
		r += TAB + indent + "'matrix' : " + s + string(",\n");
	}
		
	if( color != Color() )
	{
		string s = "[";
		s += floatToString(color.r) + ", " +
			 floatToString(color.g) + ", " +
			 floatToString(color.b) + ", " +
			 floatToString(color.a);
		s += "]";
		r += TAB + indent + "'color' : " + s + string(",\n");
	}
	
	r += TAB + indent + "'visible' : " + (visible ? "1" : "0") + string(",\n");
	
	return r + serializeChildren(indent);
}

string Node::serializeChildren(string indent) const
{
	string r;
	if(!children.empty())
	{
		r += TAB + indent + "'children' : [\n";
		for(vector<Node*>::const_iterator itr = children.begin(); itr!=children.end(); itr++)
			r += (*itr)->serialize(TAB + TAB + TAB + indent) + ",\n";
		r += TAB + TAB + indent + "],\n";
	}
	return r;
}

void Node::handleChild(const parse::Node* n)
{
	const parse::Node* value = n->data.value;
	string n_name = n->data.s;
	
	if(n_name == "name")
		name = value->data.s;
	
	if(n_name == "visible")
		visible = value->data.i ? true : false;
	
	if(n_name == "matrix")
	{
		double v[16];
		int i = 0;
		for(vector<parse::Node*>::const_iterator itr = value->children.begin();
			itr!=value->children.end() && i < 16;
			itr++)
		{
			v[i++] = (*itr)->data.x;
		}
		matrix.set(v);
	}
	
	if(n_name == "color")
	{
		double v[4];
		int i = 0;
		for(vector<parse::Node*>::const_iterator itr = value->children.begin();
			itr!=value->children.end() && i < 4;
			itr++)
		{
			v[i++] = (*itr)->data.x;
		}
		color.set(v[0], v[1], v[2], v[3]);
	}
	
	if(n_name == "children")
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
			else if(type == "String")
				newNode = new String();
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

bool Node::vectorInside( const Vec2& /*V*/ ) const
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
				node->worldMatrix = worldMatrix * node->matrix;
				node->worldColor = worldColor * node->color;
				(*itr)->keyboard(inkey);
			}
		}
	}
}

bool Node::mouseDown(const Vec2& C)
{
	if( !visible )
		return false;
	
	if( delegate && vectorInside(C) )
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
				node->worldMatrix = worldMatrix * node->matrix;
				node->worldColor = worldColor * node->color;
				if( (*itr)->mouseDown(C) )
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
			{
				node->worldMatrix = worldMatrix * node->matrix;
				node->worldColor = worldColor * node->color;
				(*itr)->mouseDragged(C);
			}
		}
	}
}

void Node::mouseUp(const Vec2& C)
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
				node->worldMatrix = worldMatrix * node->matrix;
				node->worldColor = worldColor * node->color;
				(*itr)->mouseUp(C);
			}
		}
	}
}

const Mat4& Node::getWorldMatrix() const
{
	return worldMatrix;
}

const Color& Node::getWorldColor() const
{
	return worldColor;
}

Actor* Node::actorInClick(const Vec2& C)
{
	if(!visible)
		return NULL;
	
	for(vector<Node*>::reverse_iterator itr = children.rbegin(); itr!=children.rend(); itr++)
	{
		Actor* n = (*itr)->actorInClick(C);
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


Mesh::Mesh() : elementType(kTriangles),
			   numberOfVertices(0),
			   numberOfElements(0),
			   positions(NULL),
			   indices(NULL)
{
	
}

Mesh::~Mesh()
{
	if(positions)
		delete[] positions;
	if(indices)
		delete[] indices;
}

void Mesh::resize(int inNumberOfVertices, int inNumberOfElements)
{
	if(positions)
		delete[] positions;
	if(indices)
		delete[] indices;
	
	numberOfVertices = inNumberOfVertices;
	numberOfElements = inNumberOfElements;
	
	int sd = (elementType == kTriangles) ? 3 : 2;
	
	positions = new float[3 * numberOfVertices];
	indices = new short[sd * numberOfElements];
}

Polygon::Polygon() : drawType(kSolid), valid(false), mesh(NULL)
{
	type = "Polygon";
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

Polygon& Polygon::operator=(const Polygon& P)
{
	this->vector<Vec2>::operator=(P);
	this->Node::operator=(P);
	this->mesh = NULL;
	this->valid = false;	
	return *this;
}

Polygon& Polygon::add( const Vec2& V )
{	
	push_back(V);
	return *this;
}

Polygon& Polygon::add( double x, double y )
{
	push_back(Vec2(x,y));
	return *this;
}


Polygon& Polygon::operator += (const Vec2& V) {
	for( iterator itr=begin(); itr!=end(); itr++ )
		*itr+=V;
	return *this;
}

Polygon& Polygon::operator -= (const Vec2& V) {
	for( iterator itr=begin(); itr!=end(); itr++ )
		*itr-=V;
	return *this;
}

Polygon& Polygon::operator *= (double k) {
	for( iterator itr=begin(); itr!=end(); itr++ )
		*itr*=k;
	return *this;
}

Polygon& Polygon::operator /= (double k) {
	for( iterator itr=begin(); itr!=end(); itr++ )
		*itr/=k;
	return *this;
}

Polygon Polygon::operator+ (const Vec2& V) const {
	return Polygon(*this)+=V;
}

Polygon Polygon::operator- (const Vec2& V) const {
	return Polygon(*this)-=V;
}

Polygon Polygon::operator* (double k) const {
	return Polygon(*this)*=k;
}

Polygon Polygon::operator/ (double k) const {
	return Polygon(*this)/=k;
}

Polygon operator * (const Mat4& M, const Polygon& P) {
	Polygon R;
	R.drawType = P.drawType;
	
	int n = P.size();
	
	vector<Vec2> r(n);
	
	for( int i = 0; i < n; i++)
	{
		Vec4 v = M * Vec4(P[i], 0.0, 1.0);
		r[i] = (Vec2(v.x, v.y) / v.w);
	}
	
	R.setVertices(r);
	return R;
}

void Polygon::display() const {
	for( const_iterator itr=begin(); itr!=end(); itr++ )
		itr->display();
}

void Polygon::print() const {
	for( const_iterator itr=begin(); itr!=end(); itr++ )
	{
		itr->print();
		printf( ", " );
	}
}

void Polygon::draw() const
{
	if( size() < 3 )
		return;
	
	update();
	
	if( Sprite::renderer && mesh )
		Sprite::renderer->drawMesh(mesh, this);
	else
	{
		printf( "Attempt to draw polygon with no renderer.\n" );
		exit(0);
	}
}

vector<Vec2>::size_type Polygon::size() const
{
	return vector<Vec2>::size();
}

bool Polygon::empty() const
{
	return vector<Vec2>::empty();
}

void Polygon::rotate(double theta)
{
	double s = sin(theta), c = cos(theta);
	for(iterator itr = begin(); itr!= end(); itr++)
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

bool Polygon::vectorInside(const Vec2& V) const
{
	double theta = 0;
	int n = size();
	for( int i=0; i<n; i++ )
		theta += angleAToB( (*this)[i] - V, (*this)[(i+1)%n] - V );
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
			printf( "drawtype unknown: %d\n", drawType );
			exit(0);
		break;
	}
	
	int i = 0;
	for(const_iterator itr = begin(); itr!=end(); itr++)
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
				mesh->indices[2*i] = (short)i;
				mesh->indices[2*i + 1] = (short)((i+1)%n);
			}
		break;
		
		default:
			printf( "drawtype unknown: %d\n", drawType );
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
		const Vec2 &a((*this)[p[i]]), &b((*this)[p[(i+1) % n]]), &c((*this)[p[(i+2) % n]]);
		Mat2 T(b-a, c-a);
		
		// If it's going clockwise, it's not an ear.
		if( T.det() < 0.0 )
			continue;
		
		// If another point of the polygon is inside, it's not an ear.
		bool empty = true;
		for( int j = (i+3)%n; j!=i; j=(j+1)%n )
		{
			Vec2 v = T * (*this)[p[j]] + a;
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
			mesh->indices[k++] = (short)(p[i]);
			mesh->indices[k++] = (short)(p[(i+1)%n]);
			mesh->indices[k++] = (short)(p[(i+2)%n]);
			
			for( int j = (i+1)%n; j < n-1; j++ )
				p[j] = p[j+1];
			
			i = -1; // Recall i++ happens at the end of the loop-body.
			n--;
		}
	}
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
			push_back(Vec2((*itr)->children[0]->data.x, (*itr)->children[1]->data.x));
		}
	}
}

string Polygon::serializeElements(string indent) const
{
	string r = Node::serializeElements(indent);
	
	if( !empty() )
	{
		r += TAB + indent + "'vertices' : " + "[";
		for(const_iterator itr = begin(); itr!=end(); itr++)
			r += "[" + floatToString(itr->x) + ", " + floatToString(itr->y) + "], ";
		r += "],\n";
	}
	
	return r;
}

void Polygon::reverse()
{
	vector<Vec2> r;
	
	int n = size();
	for( int i=0; i<n; i++ )
		r.push_back((*this)[n-i-1]);
	
	setVertices(r);
}

void Polygon::setVertices(const vector<Vec2>& v)
{
	this->vector<Vec2>::operator=(v);
	valid = false;
}

vector<Vec2> Polygon::getVertices() const
{
	return *this;
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

Actor::Actor() : sprite(NULL), mesh(NULL)
{
	type = "Actor";
	tare();
}

Actor::Actor(Sprite* insprite) : mesh(NULL)
{
	type = "Actor";
	sprite = insprite;
	tare();
}

Actor::~Actor()
{
	
}

void Actor::tare()
{
	frame = 0;
	k = 1.0;
	visible = true;
	listening = true;
}

void Actor::removeSprite(const Sprite* s)
{
	if( sprite == s )
		sprite = NULL;
	
	Node::removeSprite(s);
}

void Actor::draw() const
{
	Mat4 matrix(worldMatrix);
	
	Mat3 texMatrix;
	
	if( sprite )
	{
		matrix = matrix * sprite->getOffsetMatrix(x, y, k);
		texMatrix = sprite->getTexMatrix(frame);
	}
	
	Sprite::renderer->drawMesh(mesh, matrix, texMatrix, worldColor, sprite);
	
	if( Sprite::drawLines )
		collisionPolygon().draw();
}

string Actor::serializeElements(string indent) const
{
	string r = Node::serializeElements(indent);
	if( sprite )
		r += TAB + indent + "'spriteName' : " + string("'") + sprite->name + "',\n";
	if( x )
		r += TAB + indent + "'x' : " + floatToString(x) + ",\n";
	if( y )
		r += TAB + indent + "'y' : " + floatToString(y) + ",\n";
	if( k!=1.0 )
		r += TAB + indent + "'k' : " + floatToString(k) + ",\n";
	if( frame )
		r += TAB + indent + "'frame' : " + intToString(frame) + ",\n";
	
	if( !polygon.empty() )
	{	
		r += TAB + indent + "'polygon' : " + polygon.serialize();
		r += "\n";
	}
	
	return r;
}

string Actor::serialize(string indent) const
{
	return indent + flattenWhitespace(Node::serialize().c_str());
}


map<Actor*, string> gActorToSpriteName;
map<String*, string> gStringToFontName;

void Actor::handleChild(const parse::Node* n)
{
	Node::handleChild(n);
	
	string n_name = n->data.s;
	parse::Node* value = n->data.value;
	
	if(value)
	{
		if(n_name == "x")
			x = value->data.x;
		
		if(n_name == "y")
			y = value->data.x;
		
		if(n_name == "k")
			k = value->data.x;
		
		if(n_name == "frame")
			frame = value->data.i;
		
		if(n_name == "spriteName")
			gActorToSpriteName[this] = value->data.s;
	}
	
	if(n_name == "polygon")
		polygon.initWithParseNode(value);
}

Polygon Actor::collisionPolygon() const
{
	Polygon R;
	
	if( !polygon.empty() )
	{
		R = k * polygon + Vec2(x, y);
	}
	else if( sprite )
	{
		double w = k * sprite->width / sprite->numberOfColumns,
			   h = k * sprite->height / sprite->numberOfRows;
		
		if( !sprite->polygon.empty() )
		{
			R = k * sprite->polygon + Vec2(x,y);
			if( sprite->center )
				R -= 0.5 * Vec2(w, h);
		}
		else
		{	
			double x0 = x, x1 = x+w, y0 = y, y1 = y+h;
			
			R.add(x0, y0).add(x1, y0).add(x1, y1).add(x0, y1);
			
			if( sprite->center )
				R -= 0.5 * Vec2(w, h);
		}
	}
	
	R.setDrawType(Polygon::kOutline);
	return worldMatrix * R;
}

bool Actor::vectorInside(const Vec2& C) const
{
	return collisionPolygon().vectorInside(Vec2(C.x, C.y));
}

Actor* Actor::actorInClick(const Vec2& C)
{
	if(!visible)
		return NULL;
	
	if( vectorInside(C) )
		return this;
	return NULL;
}

Button::Button() : baseFrame(0),
				   depressed(false),
				   handler(NULL)
{
	type = "Button";
}

Button::Button(Sprite* insprite) : Actor(insprite),
								   baseFrame(0),
								   depressed(false),
								   handler(NULL)
{
	type = "Button";
	frame = 0;
}

Button::Button(Sprite* insprite, int baseFrame) : Actor(insprite),
												  baseFrame(baseFrame),
												  depressed(false),
												  handler(NULL)
{
	type = "Button";
	frame = baseFrame;
}

void Button::draw() const
{
	if( !sprite )
	{
		printf( "Button %s drawing with null sprite.\n", name.c_str() );
		exit(0);
	}
	
	Actor::draw();
	
	if( Sprite::drawLines )
		collisionPolygon().draw();
}

bool Button::mouseDown(const Vec2& C)
{
	if( !visible )
		return false;
	
	frame = baseFrame;
	
	if( vectorInside(C) )
	{
		frame++;
		depressed = true;
		if( handler )
			handler->down(this);
		return true;
	}
	return false;
}

void Button::mouseDragged(const Vec2& C)
{
	if( !visible )
		return;
	
	frame = baseFrame;
	if( vectorInside(C) )
	{
		frame++;
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

void Button::mouseUp(const Vec2& C)
{
	if( !visible )
		return;
	
	if( vectorInside(C) )
	{
		if( handler )
			handler->click(this);
		else
			printf( "WARNING: button %s with no handler.\n", name.c_str() );
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


String::String() : font(NULL),
				   justification("left"),
				   drawPipe(false)
{
	type = "String";
}

String::String(Font* infont) : Actor(infont),
							   font(infont),
							   justification("left"),
							   drawPipe(false)
{
	type = "String";
}

void String::setString(const std::string& ins)
{
	s = ins;
}

void String::draw() const
{
	if( font )
	{
		string news = s+(drawPipe?"|":"");
		font->drawString(worldMatrix, worldColor, x, y, k,
			news.c_str(), justification);
		if( Sprite::drawLines )
			collisionPolygon().draw();
	}
	else
	{
		printf( "Attempt to draw string with no font.\n" );
		printf( "string = %s fontName = %s\n", s.c_str(), fontName.c_str() );
		exit(0);
	}
}

string String::serializeElements(string indent) const
{
	string r = Node::serializeElements(indent);
	if( font )
		r += TAB + indent + "'fontName' : " + string("'") + font->name + "',\n";
	if( x )
		r += TAB + indent + "'x' : " + floatToString(x) + ",\n";
	if( y )
		r += TAB + indent + "'y' : " + floatToString(y) + ",\n";
	if( k!=1.0 )
		r += TAB + indent + "'k' : " + floatToString(k) + ",\n";
	if( s != "" )
		r += TAB + indent + "'s' : '" + s + "',\n";
	if( justification != "left" )
		r += TAB + indent + "'justification' : '" + justification + "',\n";
	return r;
}

void String::handleChild(const parse::Node* n)
{
	Actor::handleChild(n);
	
	string n_name = n->data.s;
	parse::Node* value = n->data.value;
	
	if(value)
	{
		if(n_name == "fontName")
		{
			fontName = value->data.s;
			gStringToFontName[this] = value->data.s;
		}
		
		if(n_name == "s")
			s = value->data.s;
		
		if(n_name == "justification")
			justification = value->data.s;
	}
}

Polygon String::collisionPolygon() const
{
	Polygon R;
	
	if( !polygon.empty() )
	{
		R = polygon + Vec2(x, y);
	}
	if( font )
	{
		R = worldMatrix * (font->stringRectangle(k, s.c_str(), justification) + Vec2(x, y));
	}
	else
	{
		printf( "Attempt to compute string rectangle with no font.\n" );
		exit(0);
	}
	
	R.setDrawType(Polygon::kOutline);
	return R;
}

void String::keyboard(unsigned char inkey)
{
	if( delegate )
		delegate->keyboard(inkey);
}

Integer::Integer() : ptr(NULL)
{
	type = "Integer";
}

Integer::Integer(Font* infont, int* inptr) : String(infont),
											 ptr(inptr)
{
	type = "Integer";
}

void Integer::draw() const
{
	if( ptr )
	{
		font->drawString(worldMatrix, worldColor, x, y, k,
			intToStringWithCommas(*ptr).c_str(), justification);
	}
	else
		String::draw();
}

void Integer::handleChild(const parse::Node* node)
{
	String::handleChild(node);
}

string Integer::serializeElements(std::string indent) const
{
	return String::serializeElements(indent);
}


Layer::Layer() {type = "Layer";}

World::World() : bank(NULL)
{
#if !defined(STUB_SOUND)
	context = new Context();
	destroySoundQueue();
#endif
	type = "World";
}

World::~World()
{
#if !defined(STUB_SOUND)
	delete context;
#endif
	
	for(vector<Sprite*>::iterator itr = sprites.begin(); itr!=sprites.end(); itr++)
		delete *itr;

#if !defined(STUB_SOUND)
	for(vector<Sound*>::iterator itr = sounds.begin(); itr!=sounds.end(); itr++)
		delete *itr;
#endif
}

void World::draw() const
{
	if( !Sprite::renderer )
	{
		printf( "Attempt to draw with no renderer world: %s\n", name.c_str() );
		exit(0);
	}
	worldMatrix = matrix;
	worldColor = color;
	Node::draw();
}

void World::resize(int width, int height)
{
	if( !Sprite::renderer )
	{
		printf( "Attempt to resize with no renderer world: %s\n", name.c_str() );
		exit(0);
	}
	
	Sprite::renderer->projection = Mat4(
		2.0 / width, 0.0, 0.0, 0.0,
		0.0, 2.0 / height, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		-1.0, -1.0, 0.0, 1.0);
}

void World::addSprite(Sprite* sprite)
{
	sprites.push_back(sprite);
}

void World::removeSprite(const Sprite* sprite)
{
	int n = sprites.size();
	int j = 0;
	for(int i = 0; i < n; i++)
		if(sprites[i] != sprite)
		{
			sprites[j] = sprites[i];
			j++;
		}
	sprites.resize(j);
	
	Node::removeSprite(sprite);
}

void World::keyboard(unsigned char inkey)
{
	worldMatrix = matrix;
	worldColor = color;
	Node::keyboard(inkey);
}

bool World::mouseDown(const Vec2& C)
{
	worldMatrix = matrix;
	worldColor = color;
	clearTookMouseDown();
	Node::mouseDown(C);
	return false;
}

void World::mouseDragged(const Vec2& C)
{
	worldMatrix = matrix;
	worldColor = color;
	Node::mouseDragged(C);
}

void World::mouseUp(const Vec2& C)
{
	worldMatrix = matrix;
	worldColor = color;
	Node::mouseUp(C);
}

void World::initWithPath(const char* path)
{
	if(bank)
		bank->initSerializableWithPath(this, path);
	else
	{
		printf( "World initiailzied with no bank.\n" );
		exit(0);
	}
}

void World::initWithParseNode(const parse::Node* n)
{
	gActorToSpriteName.clear();
	gStringToFontName.clear();
	
	Node::initWithParseNode(n);
	
	updateSpriteMap();
	updateSoundMap();
	
	updateNodeMap();
	
	for(map<Actor*, string>::iterator itr = gActorToSpriteName.begin();
		itr != gActorToSpriteName.end();
		itr++)
	{
		const string& spriteName(itr->second);
		if( spriteName != "" )
		{
			map<string, Sprite*>::iterator mitr = spriteMap.find(spriteName);
			if( mitr != spriteMap.end() )
				itr->first->sprite = mitr->second;
			else
			{
				printf( "Actor %s requested sprite not found: %s\n",
						itr->first->name.c_str(), spriteName.c_str() );
				exit(0);
			}
		}
	}
	
	for(map<String*, string>::iterator itr = gStringToFontName.begin();
		itr != gStringToFontName.end();
		itr++)
	{
		const string& fontName(itr->second);
		if( fontName != "" )
		{
			map<string, Sprite*>::iterator mitr = spriteMap.find(fontName);
			if( mitr != spriteMap.end() )
				itr->first->font = (Font*)(mitr->second);
		}
	}
}

void World::updateSpriteMap()
{
	spriteMap.clear();
	for(vector<Sprite*>::iterator itr = sprites.begin(); itr!=sprites.end(); itr++)
	{
		Sprite* sprite = *itr;
		spriteMap[sprite->name] = sprite;
	}
}

void World::updateSoundMap()
{
#if !defined(STUB_SOUND)
	soundMap.clear();
	for(vector<Sound*>::iterator itr = sounds.begin(); itr!=sounds.end(); itr++)
	{
		Sound* sound = *itr;
		soundMap[sound->name] = sound;
	}
#endif
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
	Node::handleChild(n);
	
	string n_name = n->data.s;
	parse::Node* value = n->data.value;
	
	if( value )
	{
		if( n_name == "sprites" )
		{
			for(vector<parse::Node*>::const_iterator itr = value->children.begin();
				itr!=value->children.end();
				itr++)
			{
				string type = getType(*itr);
				Sprite* sprite = NULL;
				
				if(type=="Sprite")
					sprite = new Sprite();
				else if(type == "Font")
					sprite = new Font();
				else
				{
					printf("Object in sprite list must be type Sprite or Font.\n");
					exit(0);
				}
				
				if( sprite )
				{
					sprite->initWithParseNode(*itr);
					sprites.push_back(sprite);
					deleteResources.push_back(sprite);
					
					bank->initTextureWithPath(sprite, sprite->file.c_str());
				}
			}
		}
		
#if !defined(STUB_SOUND)
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
					printf("Object in sound list must be type Sound.\n");
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
		}
#endif
	}
}

void World::clear()
{
	clearChildren();
	
	sprites.clear();
#if !defined(STUB_SOUND)
	sounds.clear();
#endif
	
	for( vector<Serializable*>::iterator itr = deleteResources.begin();
		 itr != deleteResources.end();
		 itr++ )
	{
		delete *itr;
	}
	
	deleteResources.clear();
}

string World::serializeElements(string indent) const
{
	string r = Node::serializeElements(indent);
	
	r += TAB + indent + "'sprites' : [\n";
	for(vector<Sprite*>::const_iterator itr = sprites.begin(); itr!=sprites.end(); itr++)
		r += (*itr)->serialize(TAB + TAB + TAB + TAB + indent) + ",\n";
	r += TAB + TAB + indent + "],\n";
	
#if !defined(STUB_SOUND)
	r += TAB + indent + "'sounds' : [\n";
	for(vector<Sound*>::const_iterator itr = sounds.begin(); itr!=sounds.end(); itr++)
		r += (*itr)->serialize(TAB + TAB + TAB + TAB + indent) + ",\n";
	r += TAB + TAB + indent + "],\n";
#endif

	return r;
}


int gTimes = 0;
std::string gLastName;


void World::playSound(const std::string& /*name*/) const
{
#if !defined(STUB_SOUND)
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
				sound->useSource(source);
				sound->play();
				break;
			}
		}
	}
#endif
}

Node* World::getNode(const string& name)
{
	map<string, Node*>::iterator itr = nodeMap.find(name);
	if( itr != nodeMap.end() )
		return itr->second;
	return NULL;
}

Sprite* World::getSprite(const string& name)
{
	map<string, Sprite*>::iterator itr = spriteMap.find(name);
	if( itr != spriteMap.end() )
		return itr->second;
	return NULL;
}

#if !defined(STUB_SOUND)
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
#endif

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
				printf( "Animator event stopped counter dropped below 0.\n" );	
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


void Animator::step()
{
	double t = currentTime();
	
	vector<Animation*> beginMe, advanceMe, endMe;
	
	for(set<Animation*>::iterator itr = S.begin(); itr!= S.end(); itr++)
	{
		Animation* a = *itr;
		
		if(t > a->start)
		{
			if(!a->running)
				beginMe.push_back(a);
			
			advanceMe.push_back(a);
		}
		if(!a->forever && t > a->start + a->duration)
			endMe.push_back(*itr);
	}
	
	for(vector<Animation*>::iterator itr = beginMe.begin(); itr!= beginMe.end(); itr++)
	{
		Animation* a = *itr;
		a->begin();
		a->running = true;
	}
	
	for(vector<Animation*>::iterator itr = advanceMe.begin(); itr!= advanceMe.end(); itr++)
	{
		Animation* a = *itr;
		a->advance(t);
		a->last = t;
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
	printf( "start: %f duration: %f stopsEvents: %d", start, duration, stopsEvents );
}

void Animation::display() const
{
	print();
	printf( "\n" );
}

}

