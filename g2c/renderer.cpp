
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



#include "opengl.h"
#include "sprites.h"

namespace sprite {

Renderer::Renderer()
{
	quad = new Mesh;
	
	quad->resize(4, 2);
	quad->elementType = Mesh::kTriangles;
	
	const float temp_positions[] = { 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0 };
	const short temp_indices[] = { 0, 1, 2,  0, 2, 3 };
	
	memcpy(quad->positions, temp_positions, sizeof(temp_positions));
	memcpy(quad->indices, temp_indices, sizeof(temp_indices));
}

Renderer::~Renderer()
{
	delete quad;
}

void Renderer::drawMesh(const Mesh* mesh, const Node* node) const
{
	const Mat4& matrix = node->getWorldMatrix();
	const Color& color = node->getWorldColor();
	
	const Texture* texture = NULL;
	
	if( projection == Mat4() )
	{
		printf( "WARNING: using renderer with projection = identity matrix.\n" );
	}
	
	drawMesh(mesh, matrix, Mat3(), color, texture);
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

	if( !mesh ) mesh = quad;
	
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
	
	const short* indices = mesh->indices;
	const float* positions = mesh->positions;
	
	// Determine the size of the positions array from
	// the maximum index in indices.
	int size = 0;
	for(int e = 0; e < dimension * mesh->numberOfElements; e++)
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
    
    glDrawElements(gltype, dimension*mesh->numberOfElements, GL_UNSIGNED_SHORT, indices);
    
    delete[] texCoords;
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
#endif
}



RendererGL2::RendererGL2() :
	initialized(false),
	program(0),
	vertexShader(0),
	fragmentShader(0),
	defaultTexture(NULL)
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
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), quad->positions, GL_STREAM_DRAW);
	
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(short), quad->indices, GL_STREAM_DRAW);
	
	glGenBuffers(1, &polygonBuffer);
	glGenBuffers(1, &polygonIndexBuffer);
	
	const char* vertexCode = "\n"
		"	attribute vec3 position;\n"
		"	uniform mat4 matrix;\n"
		"	uniform mat3 texMatrix;\n"
		"	varying vec2 v_texcoord;\n"
		"	void main() {\n"
		"		vec3 t = texMatrix * vec3(position.xy, 1.0);\n"
		"		v_texcoord = t.xy / t.z;\n"
		"		gl_Position = matrix * vec4(position, 1.0);\n"
		"	}\n";
	
	const char* fragmentCode = "\n"
		"#ifdef GL_ES\nprecision highp float;\n#endif\n"
		"	uniform vec4 color;\n"
		"	uniform sampler2D texture;\n"
		"	varying vec2 v_texcoord;\n"
		"	void main() {\n"
		"		gl_FragColor = color * texture2D(texture, v_texcoord);\n"
		"	}\n";
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);
	
	GLint logLength;
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
	if( logLength > 0 )
	{
		GLchar *log = (GLchar *)malloc(logLength);
		glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
		printf("Shader compile log:\n%s", log);
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
		printf("Shader compile log:\n%s", log);
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
		printf( "Didn't link\n" );
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

void RendererGL2::drawMesh(const Mesh* mesh,
						   const Mat4& matrix,
						   const Mat3& texMatrix,
						   const Color& color,
						   const Texture* texture) const
{
	if( !initialized )
	{
		printf( "RendererGL2 draw called before initialization\n" );
		exit(0);
	}
	
	if( !mesh ) mesh = quad;
	
	if( mesh == quad )
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, polygonBuffer);
		glBufferData(GL_ARRAY_BUFFER,
			mesh->numberOfVertices*3*sizeof(float), mesh->positions, GL_STREAM_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygonIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			mesh->numberOfElements*3*sizeof(short), mesh->indices, GL_STREAM_DRAW);
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
	
	const GLfloat cf[] = {color.r, color.g, color.b, color.a};
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
	
	glDrawElements(gltype, mesh->numberOfElements*dimension, GL_UNSIGNED_SHORT, 0);
	
	glDisableVertexAttribArray(positionLocation);
	glUseProgram(0);
}

} // end namepsace sprite