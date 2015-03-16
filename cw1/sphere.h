#ifndef SPHERE_H
#define SPHERE_H

#include "types.h"
#include "drawable.h"

class Sphere : public Drawable
{
public:
	Sphere(float radius, int tess);
	~Sphere();
	
	void updateLighting(const float3& light, bool shading);
	
	void bind();
	void unbind();
	void draw();
	void drawNormals();
	
private:
	void polarVertex(float radius, float theta, float phi, int i);
	
	float* m_vertexData;
	float* m_colorData;
	
	float* m_normalVertexData;
	float* m_normalColorData;
	int m_elements;
};

#endif

