#include "castle.h"
#include "texture.h"
#include "shader.h"
#include "model.h"

Model* Castle::s_model = NULL;
Shader* Castle::s_shader;
Shader* Castle::s_shadowVolumeShader;
int Castle::s_texLoc;

Castle::Castle(const Vec3& pos)
	: m_pos(pos),
	  m_rotation(45.0)
{
	if (s_model == NULL)
	{
		s_model = new Model("castle");
		s_shader = new Shader("castle_vert", "castle_frag");
		s_shadowVolumeShader = new Shader("shadowvol_vert", "shadowvol_frag");
		s_texLoc = s_shader->uniformLocation("texture");
	}
}

Castle::~Castle()
{
}

void Castle::bind()
{
	s_shader->bind();
}

void Castle::draw()
{
	glPushMatrix();
		glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
		glRotatef(m_rotation, 0.0, 1.0, 0.0);
		
		s_model->draw(s_texLoc);
	glPopMatrix();
}

void Castle::unbind()
{
	s_shader->unbind();
}

void Castle::bindShadowVolume()
{
	s_shadowVolumeShader->bind();
}

void Castle::drawShadowVolume()
{
	glPushMatrix();
		glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
		glRotatef(m_rotation, 0.0, 1.0, 0.0);
		
		s_model->drawShadowVolume();
	glPopMatrix();
}

void Castle::unbindShadowVolume()
{
	s_shadowVolumeShader->unbind();
}

float Castle::boundingRadius() const
{
	return s_model->boundingRadius();
}



