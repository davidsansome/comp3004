#ifndef SPHERE_H
#define SPHERE_H

#include "array.h"
#include "utilities.h"

class Sphere
{
public:
	Sphere(float radius, int tess);
	~Sphere();
	
	void updateLighting(const Vec3& light, bool shading);
	
	array<Vertex, 4> quad(int i) const;
	int quadCount() const;

private:
	void polarVertex(float radius, float theta, float phi, int i);
	Vertex vertex(int i) const;
	
	float* m_vertexData;
	float* m_colorData;
	
	float* m_normalVertexData;
	float* m_normalColorData;
	int m_elements;
};

#endif
