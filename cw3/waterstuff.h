#ifndef WATERSTUFF_H
#define WATERSTUFF_H

#include <svl/SVL.h>

#include "utilities.h"

class Shader;
class Terrain;
class Texture;

struct Blob
{
	float x;
	float y;
	float z;
	float offset;
	float size;
};

class WaterStuff
{
public:
	WaterStuff();
	~WaterStuff();

	void generate(int count, Terrain* terrain, float height);
	
	void update(float timeDelta);
	
	void bind();
	void draw();
	void unbind();

private:
	void doVertex(const Blob& blob, float** v, float** t, float dx, float dy);
	
	int m_count;
	float m_height;
	Texture* m_glowTexture;
	int m_glowTexLoc;
	
	float* m_vertexBuffer;
	float* m_texCoordBuffer;
	
	Shader* m_shader;
};

#endif
