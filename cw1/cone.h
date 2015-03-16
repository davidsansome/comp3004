#ifndef CONE_H
#define CONE_H

#include "types.h"
#include "drawable.h"

class Cone : public Drawable
{
public:
	Cone(float height, float radius, int tess);
	virtual ~Cone();
	
	void updateLighting(const float3& light, bool shading);
	
	void bind();
	void unbind();
	void draw();
	void drawNormals();
	
private:
	void polarVertex(float radius, float theta, float y, int i);
	void setNormal(int i, float x, float y, float z);
	void setNormal(int i, const float3& n);
	
	float* m_vertexData;
	float* m_colorData;
	float* m_normalData;
	
	float* m_normalVertexData;
	float* m_normalColorData;
	
	int m_elements;
};

#endif

