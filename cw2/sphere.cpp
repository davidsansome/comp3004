#include "sphere.h"

Sphere::Sphere(float radius, int tess)
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

array<Vertex, 4> Sphere::quad(int i) const
{
	array<Vertex, 4> a =
	{{
		a[0] = vertex(i*2 + 0),
		a[1] = vertex(i*2 + 1),
		a[2] = vertex(i*2 + 3),
		a[3] = vertex(i*2 + 2)
	}};
	
	return a;
}

Vertex Sphere::vertex(int i) const
{
	return Utilities::newVertex
	(
		Vec3
		(
			m_vertexData[i*3 + 0],
			m_vertexData[i*3 + 1],
			m_vertexData[i*3 + 2]
		),
		Vec3
		(
			m_colorData[i*3 + 0],
			m_colorData[i*3 + 1],
			m_colorData[i*3 + 2]
		),
		vl_0
	);
}

int Sphere::quadCount() const
{
	return m_elements/2 - 1;
}

void Sphere::updateLighting(const Vec3& light, bool shading)
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
			Vec3 v
			(
				m_vertexData[i*3 + 0],
				m_vertexData[i*3 + 1],
				m_vertexData[i*3 + 2]
			);
			
			// Make sure the vector is normalized
			Vec3 vNorm = norm(v);
			
			// Simple phong model
			float ambient = 0.2;
			float diffuse = 0.7 * fmaxf(dot(light, vNorm), 0.0);
			intensity = Utilities::clamp<float>(0.0, ambient + diffuse, 1.0);
		}
		
		m_colorData[i*3 + 0] = intensity;
		m_colorData[i*3 + 1] = intensity;
		m_colorData[i*3 + 2] = intensity;
	}
}


