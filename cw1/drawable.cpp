#include "drawable.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

Keyframe::Keyframe(int t, const float3& p)
	: time(t),
	  position(p)
{
}

Drawable::Drawable()
	: m_time(0),
	  m_currentKeyframe(0)
{
	m_rotation[0] = 0.0;
	m_rotation[1] = 0.0;
	m_rotation[2] = 0.0;
	
	m_rotationSpeed[0] = 0.0;
	m_rotationSpeed[1] = 0.0;
	m_rotationSpeed[2] = 0.0;
}

void Drawable::setRotation(const float3& speed)
{
	m_rotationSpeed = speed;
}

void Drawable::addKeyframe(int time, const float3& position)
{
	m_keyframes.push_back(Keyframe(time, position));
}

void Drawable::updatePosition(int timeElapsed)
{
	if (m_keyframes.size() == 1)
	{
		glTranslatef(m_keyframes[0].position[0], m_keyframes[0].position[1], m_keyframes[0].position[2]);
	}
	else if (m_keyframes.size() > 1)
	{
		m_time += timeElapsed;
		while (true)
		{
			Keyframe current = m_keyframes[m_currentKeyframe];
			int nextKeyframe = (m_currentKeyframe + 1) % m_keyframes.size();
			Keyframe next = m_keyframes[nextKeyframe];
			
			if (m_time > current.time)
			{
				m_time -= current.time;
				m_currentKeyframe = nextKeyframe;
				continue;
			}
			
			float a = float(m_time) / float(current.time);
			float b = 1.0 - a;
			
			glTranslatef(current.position[0] * a + next.position[0] * b,
						current.position[1] * a + next.position[1] * b,
						current.position[2] * a + next.position[2] * b);
			break;
		}
	}
	
	float timeDelta = float(timeElapsed) / 1000.0;
	
	m_rotation[0] += m_rotationSpeed[0] * timeDelta;
	m_rotation[1] += m_rotationSpeed[1] * timeDelta;
	m_rotation[2] += m_rotationSpeed[2] * timeDelta;
	
	while (m_rotation[0] > 360.0) m_rotation[0] -= 360.0;
	while (m_rotation[1] > 360.0) m_rotation[1] -= 360.0;
	while (m_rotation[2] > 360.0) m_rotation[2] -= 360.0;
	
	glRotatef(m_rotation[0], 1.0, 0.0, 0.0);
	glRotatef(m_rotation[1], 0.0, 1.0, 0.0);
	glRotatef(m_rotation[2], 0.0, 0.0, 1.0);
}

