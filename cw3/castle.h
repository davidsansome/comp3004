#ifndef CASTLE_H
#define CASTLE_H

#include "utilities.h"
#include "solidobject.h"

class Texture;
class Model;
class Shader;

class Castle : public SolidObject
{
public:
	Castle(const Vec3& pos);
	~Castle();
	
	static void bind();
	void draw();
	static void unbind();
	
	static void bindShadowVolume();
	void drawShadowVolume();
	static void unbindShadowVolume();
	
	Vec3 pos() const { return m_pos; }
	float boundingRadius() const;
	
private:
	static Model* s_model;
	static Shader* s_shader;
	static Shader* s_shadowVolumeShader;
	static int s_texLoc;
	
	Vec3 m_pos;
	float m_rotation;
};

#endif
