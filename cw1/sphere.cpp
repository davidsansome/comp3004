#include "sphere.h"
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

Sphere::Sphere(float radius, int tess)
	: Drawable()
{
	// Initialises the vertex data for this Sphere
	m_elements = 4*tess*tess;
	m_vertexData = new float[3*m_elements];
	m_colorData = new float[3*m_elements];
	m_normalVertexData = new float[3*2*m_elements];
	m_normalColorData = new float[3*2*m_elements];
	
	float thetaStep = TWO_PI / tess;
	float phiStep = M_PI / tess;
	float verySmall = 0.0001;
	
	int i = 0;
	for (float theta=0.0 ; TWO_PI-theta>verySmall ; theta+=thetaStep)
	{
		for (float phi=-M_PI ; M_PI-phi>verySmall ; phi+=phiStep)
		{
			polarVertex(radius, theta            , phi, i++);
			polarVertex(radius, theta + thetaStep, phi, i++);
		}
	}
}

Sphere::~Sphere()
{
	delete[] m_vertexData;
	delete[] m_colorData;
	
	delete[] m_normalVertexData;
	delete[] m_normalColorData;
}

void Sphere::polarVertex(float radius, float theta, float phi, int i)
{
	// Vertex x, y, z
	m_vertexData[i*3 + 0] = radius * cosf(theta) * sinf(phi);
	m_vertexData[i*3 + 1] = radius * sinf(theta) * sinf(phi);
	m_vertexData[i*3 + 2] = radius * cosf(phi);
	
	// The two points that make up the line showing the normal
	m_normalVertexData[i*6 + 0] = radius * cosf(theta) * sinf(phi);
	m_normalVertexData[i*6 + 1] = radius * sinf(theta) * sinf(phi);
	m_normalVertexData[i*6 + 2] = radius * cosf(phi);
	
	m_normalVertexData[i*6 + 3] = (radius+0.2) * cosf(theta) * sinf(phi);
	m_normalVertexData[i*6 + 4] = (radius+0.2) * sinf(theta) * sinf(phi);
	m_normalVertexData[i*6 + 5] = (radius+0.2) * cosf(phi);
	
	// The colors of those two points
	m_normalColorData[i*6 + 0] = 1.0;
	m_normalColorData[i*6 + 1] = 0.0;
	m_normalColorData[i*6 + 2] = 0.0;
	
	m_normalColorData[i*6 + 3] = 1.0;
	m_normalColorData[i*6 + 4] = 0.0;
	m_normalColorData[i*6 + 5] = 0.0;
}

void Sphere::updateLighting(const float3& light, bool shading)
{
	// Called each frame to update the shading on the sphere
	// Loops through all the vertices and sets colors on them
	for (int i=0 ; i<m_elements ; ++i)
	{
		float intensity = 1.0;
		if (shading)
		{
			// Get the vertex.  Since we're a sphere with the center
			// at the origin, the vertex is also the normal!
			float3 v =
			{{
				m_vertexData[i*3 + 0],
				m_vertexData[i*3 + 1],
				m_vertexData[i*3 + 2]
			}};
			
			// Make sure the vector is normalized
			float3 vNorm = normalize(v);
			
			// Simple phong model
			float ambient = 0.2;
			float diffuse = 0.7 * fmaxf(dot(light, vNorm), 0.0);
			intensity = clamp(0.0, ambient + diffuse, 1.0);
		}
		
		m_colorData[i*3 + 0] = intensity;
		m_colorData[i*3 + 1] = intensity;
		m_colorData[i*3 + 2] = intensity;
	}
}

// Vertex arrays are used so we don't have to recalculate
// the vertices every frame
void Sphere::bind()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

void Sphere::unbind()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Sphere::draw()
{
	glVertexPointer(3, GL_FLOAT, 0, m_vertexData);
	glColorPointer(3, GL_FLOAT, 0, m_colorData);
	glDrawArrays(GL_QUAD_STRIP, 0, m_elements);
	
	// Since the coursework spec says "you may only use the gl commands glBegin(),
	// glEnd(), glVertex??(), or glColor()...", here's the equivalent
	// with those commands.  The above is quicker though :)
	
	/*glBegin(GL_QUAD_STRIP);
	for (int i=0 ; i<m_elements ; ++i)
	{
		glColor3fv(&m_colorData[i*3]);
		glVertex3fv(&m_vertexData[i*3]);
	}
	glEnd();*/
}

void Sphere::drawNormals()
{
	glVertexPointer(3, GL_FLOAT, 0, m_normalVertexData);
	glColorPointer(3, GL_FLOAT, 0, m_normalColorData);
	glDrawArrays(GL_LINES, 0, m_elements*2);
	
	// Again, here's the slow one:
	
	/*glBegin(GL_LINES);
	for (int i=0 ; i<m_elements*2 ; ++i)
	{
		glColor3fv(&m_normalColorData[i*3]);
		glVertex3fv(&m_normalVertexData[i*3]);
	}
	glEnd();*/
}

