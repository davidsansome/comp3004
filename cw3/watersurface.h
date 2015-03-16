#ifndef WATERSURFACE_H
#define WATERSURFACE_H

#include "utilities.h"
#include <svl/SVL.h>

class Terrain;
class Shader;
class Texture;

class WaterSurface
{
public:
	WaterSurface(Terrain* terrain, float height);
	~WaterSurface();
	
	void finalize();

	void setParams(float t, const Vec3& lightPos);
	void bind();
	void draw();
	void unbind();

private:
	void setVertex(float** v, float** t, float tx, float ty, int w, int h);
	
	Shader* m_shader;
	Texture* m_noiseTex;
	int m_noiseTexLoc;
	int m_tLoc;
	int m_lightPosLoc;
	
	float m_t;
	Vec3 m_lightPos;
	
	uint m_bufferObjects[2];
	Terrain* m_terrain;
	int m_elements;
	float m_height;
};

#endif
