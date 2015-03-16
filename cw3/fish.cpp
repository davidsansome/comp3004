#include "fish.h"
#include "model.h"
#include "shader.h"
#include "terrain.h"

Model* Fish::s_model = NULL;
Shader* Fish::s_shader;
Shader* Fish::s_shadowVolumeShader;
Terrain* Fish::s_terrain;

float Fish::s_t;
float Fish::s_lastT;

int Fish::s_tLoc;
int Fish::s_rotationModLoc;
int Fish::s_terrainSizeLoc;

Fish::Fish(Terrain* terrain, int path)
{
	if (!s_model)
	{
		s_model = new Model("terrorfish");
		s_shader = new Shader("fish_vert", "fish_frag");
		s_shadowVolumeShader = new Shader("shadowvol_vert", "shadowvol_frag");
		s_terrain = terrain;
		
		s_t = 0.0;
		
		s_tLoc = s_shader->uniformLocation("t");
		s_rotationModLoc = s_shader->uniformLocation("rotationMod");
		s_terrainSizeLoc = s_shader->uniformLocation("terrainSize");
	}
	
	switch(path)
	{
	case 0:
		m_path.addPoint(Vec3(12.0, 6.0, 70.0), Vec3(-15.0, 5.0, -15.0));
		m_path.addPoint(Vec3(18.0, 7.0, 20.0), Vec3(10.0, 0.0, 2.0));
		m_path.addPoint(Vec3(28.0, 8.0, 50.0), Vec3(7.0, 0.0, 12.0));
		m_path.addPoint(Vec3(48.0, 6.0, 65.0), Vec3(0.0, 0.0, 10.0));
		break;
	case 1:
		m_path.addPoint(Vec3(32.0, 7.5, 12.0), Vec3(7.0, -1.0, -3.0));
		m_path.addPoint(Vec3(53.0, 5.0, 28.0), Vec3(5.0, 0.0, 5.0));
		m_path.addPoint(Vec3(55.0, 7.5, 40.0), Vec3(-5.0, 1.0, 5.0));
		m_path.addPoint(Vec3(10.0, 10.0, 50.0), Vec3(-5.0, 0.0, -10.0));
		break;
	case 2:
		m_path.addPoint(Vec3(57.0, 12.0, 57.0), Vec3(-7.0, 0.0, 0.0));
		m_path.addPoint(Vec3(45.0, 8.0, 32.0), Vec3(-1.0, 5.0, -10.0));
		m_path.addPoint(Vec3(10.0, 12.0, 12.0), Vec3(10.0, 0.0, -1.0));
		m_path.addPoint(Vec3(75.0, 7.0, 30.0), Vec3(0.0, 0.0, 15.0));
	}
	m_path.finalize();
}

Fish::~Fish()
{
}

void Fish::setParams(float t)
{
	s_t = t;
}

void Fish::update(float timeDelta)
{
	m_path.updateProgress(timeDelta);
	m_pos = m_path.calc();
	m_nextPos = m_path.calc(m_path.progress()+0.0001);
	m_direction = norm(m_nextPos - m_pos);
	
	m_phi = - atan2(m_direction[2], m_direction[0]) / M_PI * 180.0;
	m_theta = asin(m_direction[1]) / M_PI * 180.0;
}

void Fish::bind(float rotationMod)
{
	s_shader->bind();
	glUniform1f(s_tLoc, s_t);
	glUniform1f(s_rotationModLoc, rotationMod);
	glUniform2f(s_terrainSizeLoc, s_terrain->width()-1, s_terrain->height()-1);
	
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 0x08, 0x08);
	glEnable(GL_STENCIL_TEST);
}

void Fish::draw()
{
	glPushMatrix();
		glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
		glRotatef(m_phi, 0.0, 1.0, 0.0);
		glRotatef(m_theta, 0.0, 0.0, 1.0);
		
		s_model->draw();
	glPopMatrix();
}

void Fish::unbind()
{
	glDisable(GL_STENCIL_TEST);
	s_shader->unbind();
}

void Fish::bindShadowVolume()
{
	s_shadowVolumeShader->bind();
}

void Fish::drawShadowVolume()
{
	glPushMatrix();
		glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
		glRotatef(m_phi, 0.0, 1.0, 0.0);
		glRotatef(m_theta, 0.0, 0.0, 1.0);
		s_model->drawShadowVolume();
	glPopMatrix();
}

void Fish::unbindShadowVolume()
{
	s_shadowVolumeShader->unbind();
}

void Fish::reset()
{
	m_path.reset();
	s_t = 0.0;
	s_lastT = 0.0;
}

float Fish::boundingRadius() const
{
	return s_model->boundingRadius();
}


