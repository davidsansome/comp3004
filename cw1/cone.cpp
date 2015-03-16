#include "cone.h"
#include "utilities.h"

#include <math.h>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

using namespace std;
using namespace Utilities;

Cone::Cone(float height, float radius, int tess)
	: Drawable()
{
	// Initialises the vertex data for this cone
	m_elements = tess + 2;
	m_vertexData = new float[2*3*m_elements];
	m_colorData = new float[2*3*m_elements];
	m_normalData = new float[2*3*m_elements];
	
	m_normalVertexData = new float[4*3*m_elements];
	m_normalColorData = new float[4*3*m_elements];
	
	float thetaStep = TWO_PI / tess;
	float verySmall = 0.0001;
	int i = 0;
	const float y = -height/2.0;
	
	// Do the base of the cone first
	// Base center point
	polarVertex(0.0, 0.0, y, i);
	setNormal(i++, 0.0, -1.0, 0.0);
	
	// Base edge points
	for (float theta=0.0 ; TWO_PI+thetaStep-theta>verySmall ; theta+=thetaStep)
	{
		polarVertex(radius, theta, y, i);
		setNormal(i++, 0.0, -1.0, 0.0); // Normal points down all the time
	}
	
	// Now do the sides of the cone
	// Top point
	polarVertex(0.0, 0.0, height/2.0, i);
	setNormal(i++, 0.0, 1.0, 0.0);
	
	// Edge points
	float a = atan(height/radius);
	float b = M_PI_2 - a;
	float normalY = sinf(b) / sinf(a);  // Sine rule!
	for (float theta=0.0 ; TWO_PI+thetaStep-theta>verySmall ; theta+=thetaStep)
	{
		float3 normal =
		{{
			cosf(theta),
			normalY,
			sinf(theta)
		}};
	
		polarVertex(radius, theta, y, i);
		setNormal(i++, normalize(normal));
	}
}

Cone::~Cone()
{
	delete[] m_vertexData;
	delete[] m_colorData;
	delete[] m_normalData;
	
	delete[] m_normalVertexData;
	delete[] m_normalColorData;
}

void Cone::polarVertex(float radius, float theta, float y, int i)
{
	m_vertexData[i*3 + 0] = radius * cosf(theta);
	m_vertexData[i*3 + 1] = y;
	m_vertexData[i*3 + 2] = radius * sinf(theta);
}

void Cone::setNormal(int i, float x, float y, float z)
{
	// This is the actual normal for the vertex
	float3 normal = {{x, y, z}};
	normal = normalize(normal);
	m_normalData[i*3 + 0] = normal[0];
	m_normalData[i*3 + 1] = normal[1];
	m_normalData[i*3 + 2] = normal[2];
	
	// These are the two points of the little red line that represents
	// the normal
	m_normalVertexData[i*6 + 0] = m_vertexData[i*3 + 0];
	m_normalVertexData[i*6 + 1] = m_vertexData[i*3 + 1];
	m_normalVertexData[i*6 + 2] = m_vertexData[i*3 + 2];
	
	m_normalVertexData[i*6 + 3] = m_vertexData[i*3 + 0] + x*0.2;
	m_normalVertexData[i*6 + 4] = m_vertexData[i*3 + 1] + y*0.2;
	m_normalVertexData[i*6 + 5] = m_vertexData[i*3 + 2] + z*0.2;
	
	// And their colors
	m_normalColorData[i*6 + 0] = 1.0;
	m_normalColorData[i*6 + 1] = 0.0;
	m_normalColorData[i*6 + 2] = 0.0;
	
	m_normalColorData[i*6 + 3] = 1.0;
	m_normalColorData[i*6 + 4] = 0.0;
	m_normalColorData[i*6 + 5] = 0.0;
}

// Convenience function that takes a float3
void Cone::setNormal(int i, const float3& n)
{
	setNormal(i, n[0], n[1], n[2]);
}

void Cone::updateLighting(const float3& light, bool shading)
{
	// Called each frame to update the shading on the cone
	// Loops through all the vertices and sets colors on them
	for (int i=0 ; i<m_elements*2 ; ++i)
	{
		float intensity = 1.0;
		
		if (shading)
		{
			// Get the normal as calculated previously
			float3 norm =
			{{
				m_normalData[i*3 + 0],
				m_normalData[i*3 + 1],
				m_normalData[i*3 + 2]
			}};
			
			// Simple phong model
			float ambient = 0.2;
			float diffuse = 0.7 * fmaxf(dot(light, norm), 0.0);
			intensity = clamp(0.0, ambient + diffuse, 1.0);
		}
		
		m_colorData[i*3 + 0] = intensity;
		m_colorData[i*3 + 1] = intensity;
		m_colorData[i*3 + 2] = intensity;
	}
}

// Vertex arrays are used so we don't have to recalculate
// the vertices every frame
void Cone::bind()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

void Cone::unbind()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Cone::draw()
{
	// Base
	glVertexPointer(3, GL_FLOAT, 0, m_vertexData);
	glColorPointer(3, GL_FLOAT, 0, m_colorData);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_elements);
	
	// Sides
	glVertexPointer(3, GL_FLOAT, 0, &m_vertexData[m_elements*3]);
	glColorPointer(3, GL_FLOAT, 0, &m_colorData[m_elements*3]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_elements);
	
	// Since the coursework spec says "you may only use the gl commands glBegin(),
	// glEnd(), glVertex??(), or glColor()...", here's the equivalent
	// with those commands.  The above is quicker though :)
	
	/*int i=0;
	
	glBegin(GL_TRIANGLE_FAN);
	for (; i<m_elements ; ++i)
	{
		glColor3fv(&m_colorData[i*3]);
		glVertex3fv(&m_vertexData[i*3]);
	}
	glEnd();
	
	glBegin(GL_TRIANGLE_FAN);
	for (; i<m_elements*2 ; ++i)
	{
		glColor3fv(&m_colorData[i*3]);
		glVertex3fv(&m_vertexData[i*3]);
	}
	glEnd();*/
}

void Cone::drawNormals()
{
	glVertexPointer(3, GL_FLOAT, 0, m_normalVertexData);
	glColorPointer(3, GL_FLOAT, 0, m_normalColorData);
	glDrawArrays(GL_LINES, 0, m_elements*4);
	
	// Again, here's the slow one:
	
	/*glBegin(GL_LINES);
	for (int i=0 ; i<m_elements*4 ; ++i)
	{
		glColor3fv(&m_normalColorData[i*3]);
		glVertex3fv(&m_normalVertexData[i*3]);
	}
	glEnd();*/
}

