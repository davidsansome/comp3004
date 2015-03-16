#ifndef FISH_H
#define FISH_H

#include "bezier.h"
#include "solidobject.h"

#include <svl/SVL.h>

class Model;
class Shader;
class Terrain;

class Fish : public SolidObject
{
public:
	Fish(Terrain* terrain, int path);
	~Fish();
	
	void update(float timeDelta);
	
	static void setParams(float t);
	static void bind(float rotationMod = 1.0);
	void draw();
	static void unbind();
	
	static void bindShadowVolume();
	void drawShadowVolume();
	static void unbindShadowVolume();
	
	Vec3 pos() const { return m_pos; }
	Vec3 nextPos() const { return m_nextPos; }
	Vec3 direction() const { return m_direction; }
	
	float boundingRadius() const;
	
	void reset();

private:
	static Shader* s_shader;
	static Shader* s_shadowVolumeShader;
	static Model* s_model;
	static Terrain* s_terrain;
	
	static float s_t;
	static float s_lastT;
	
	static int s_tLoc;
	static int s_rotationModLoc;
	static int s_terrainSizeLoc;
	
	Bezier m_path;
	Vec3 m_pos;
	Vec3 m_nextPos;
	Vec3 m_direction;
	
	float m_phi;
	float m_theta;
};

#endif
