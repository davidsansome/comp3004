#ifndef OUTSIDE_H
#define OUTSIDE_H

class Terrain;
class Shader;
class Texture;

#include <svl/SVL.h>

class Outside
{
public:
	Outside(Terrain* terrain);
	~Outside();
	
	void setParams(const Vec3& lightPos);
	
	void bind();
	void draw();
	void unbind();

private:
	Shader* m_shader;
	Texture* m_woodTexture;
	
	int m_woodTextureLoc;
	int m_lightPosLoc;
	Vec3 m_lightPos;
	
	Terrain* m_terrain;
};

#endif
