#include "starfish.h"
#include "shader.h"
#include "model.h"
#include "terrain.h"

Shader* Starfish::s_shader = NULL;
Terrain* Starfish::s_terrain = NULL;
Model* Starfish::s_model;
Vec3 Starfish::s_lightPos;
int Starfish::s_texLoc;
int Starfish::s_lightPosLoc;
int Starfish::s_matrixLoc;
int Starfish::s_tLoc;
float Starfish::s_t;

Starfish::Starfish(Terrain* terrain)
{
	if (s_shader == NULL)
	{
		s_shader = new Shader("starfish_vert", "starfish_frag");
		s_terrain = terrain;
		s_model = new Model("starfish");
		s_texLoc = s_shader->uniformLocation("texture");
		s_lightPosLoc = s_shader->uniformLocation("lightPos");
		s_matrixLoc = s_shader->uniformLocation("matrix");
		s_tLoc = s_shader->uniformLocation("t");
	}
	
	m_pos = Vec2
	(
		float(rand()) / RAND_MAX * (s_terrain->width()  - 1 - boundingRadius() * 2.0) + boundingRadius(),
		float(rand()) / RAND_MAX * (s_terrain->height() - 1 - boundingRadius() * 2.0) + boundingRadius()
	);
	
	newWander();
	m_direction = float(rand()) / RAND_MAX * 360.0;
}

Starfish::~Starfish()
{
}

void Starfish::setParams(float t, const Vec3& lightPos)
{
	s_lightPos = lightPos;
	s_t = t;
}

void Starfish::bind()
{
	s_shader->bind();
	glUniform1f(s_tLoc, s_t);
}

void Starfish::draw()
{
	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glUniformMatrix4fv(s_matrixLoc, 1, false, matrix);
	glUniform3f(s_lightPosLoc, s_lightPos[0], s_lightPos[1], s_lightPos[2]);
	
	Vec3 normal = s_terrain->normalAt(m_pos);
	
	glPushMatrix();
		Vec3 p = pos();
		glTranslatef(p[0], p[1], p[2]);
		glRotatef(asin(normal[2]) / M_PI * 180.0, 1.0, 0.0, 0.0);
		glRotatef(- asin(normal[0]) / M_PI * 180.0, 0.0, 0.0, 1.0);
		//glRotatef(m_direction, 0.0, 1.0, 0.0);
		
		s_model->draw(s_texLoc);
	glPopMatrix();
}

void Starfish::unbind()
{
	s_shader->unbind();
}

float Starfish::boundingRadius() const
{
	return s_model->boundingRadius();
}

void Starfish::newWander()
{
	while (true)
	{
		m_distanceRemaining = 10.0 + float(rand()) / RAND_MAX * 5.0;
		
		float radians = float(rand()) / RAND_MAX * M_PI * 2.0;
		
		m_directionVector = Vec2(sin(radians), cos(radians));
		
		Vec2 destination(m_pos + m_directionVector * m_distanceRemaining);
		if (destination[0] > boundingRadius() &&
		    destination[1] > boundingRadius() &&
		    destination[0] < s_terrain->width() - 1 - boundingRadius() &&
		    destination[1] < s_terrain->height() - 1 - boundingRadius())
			break;
	}
	m_direction.finishedEditing();
}

void Starfish::updatePosition(int timeElapsed)
{
	float timeDelta = float(timeElapsed) / 6000.0;
	
	m_distanceRemaining -= timeDelta;
	if (m_distanceRemaining > 0.0)
		m_pos += m_directionVector * timeDelta;
	if (m_distanceRemaining < -2.0)
		newWander();
}

Vec3 Starfish::pos() const
{
	return Vec3(m_pos[0], s_terrain->heightAt(m_pos), m_pos[1]);
}


