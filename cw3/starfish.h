#ifndef STARFISH_H
#define STARFISH_H

#include "utilities.h"
#include "solidobject.h"
#include "timedependentvariable.h"

class Shader;
class Model;
class Terrain;

class Starfish : public SolidObject
{
public:
	Starfish(Terrain* terrain);
	~Starfish();
	
	static void setParams(float t, const Vec3& lightPos);
	static void bind();
	void draw();
	static void unbind();
	
	void updatePosition(int timeElapsed);
	
	Vec3 pos() const;
	float boundingRadius() const;

private:
	void newWander();
	
	static Shader* s_shader;
	static Terrain* s_terrain;
	static Model* s_model;
	static Vec3 s_lightPos;
	static int s_texLoc;
	static int s_lightPosLoc;
	static int s_matrixLoc;
	static int s_tLoc;
	static float s_t;
	
	Vec2 m_pos;
	
	TimeDependentVariable m_direction;
	Vec2 m_directionVector;
	float m_distanceRemaining;
};

#endif
