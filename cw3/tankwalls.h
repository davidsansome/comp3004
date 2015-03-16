#ifndef TANKWALLS_H
#define TANKWALLS_H

#include "utilities.h"

#include <svl/SVL.h>

class Terrain;
class Shader;

class TankWalls
{
public:
	TankWalls(Terrain* terrain, float bottom, float top);
	~TankWalls();
	
	void generate();
	void setShader(Shader* shader);
	
	void bind(const Vec3& glassColor = vl_0, const Vec3& distColor = vl_0, float alphaMod = 1.0);
	void draw();
	void unbind();

private:
	void generateHorizEdge(float** v, float** t, float** n, int y, float normalDirection, int incDirection);
	void generateVertEdge(float** v, float** t, float** n, int x, float normalDirection, int incDirection);
	
	Terrain* m_terrain;
	Shader* m_shader;
	
	uint m_bufferObjects[3];
	float m_bottom;
	float m_top;
	int m_horizCount;
	int m_vertCount;
	
	int m_terrainSizeLoc;
	int m_glassColorLoc;
	int m_distColorLoc;
	int m_alphaModLoc;
};

#endif
