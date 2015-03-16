#ifndef SEAWEED_H
#define SEAWEED_H

#include <svl/SVL.h>

class Shader;
class Model;
class Terrain;

class Seaweed
{
public:
	Seaweed(Terrain* terrain);
	~Seaweed();
	
	static void bind(float alphaMod);
	void draw();
	static void unbind();

private:
	static Shader* s_shader;
	static Model* s_model;
	
	static int s_texLoc;
	static int s_alphaModLoc;
	
	Vec3 m_pos;
	float m_rot;
	float m_size;
};

#endif
