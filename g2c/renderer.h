
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



#ifndef _RENDERER_
#define _RENDERER_

#include "opengl.h"
#include "lin/lin.h"

namespace g2c {

class Color;
class Node;
class Mesh;


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

} // end namespace

#endif
