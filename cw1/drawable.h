#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <vector>
#include "types.h"

using std::vector;

class Keyframe
{
public:
	Keyframe(int t, const float3& p);
	
	int time;
	float3 position;
};

class Drawable
{
public:
	Drawable();
	virtual ~Drawable() {}
	
	void updatePosition(int timeElapsed);
	virtual void updateLighting(const float3& light, bool shading) = 0;
	
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void draw() = 0;
	virtual void drawNormals() = 0;
	
	void setRotation(const float3& speed);
	void addKeyframe(int msec, const float3& position);

private:
	vector<Keyframe> m_keyframes;
	float3 m_rotationSpeed;
	float3 m_rotation;
	float3 m_position;
	int m_time;
	int m_currentKeyframe;
};

#endif
