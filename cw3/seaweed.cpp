#include "seaweed.h"
#include "texture.h"
#include "shader.h"
#include "model.h"
#include "terrain.h"

Shader* Seaweed::s_shader = NULL;
Model* Seaweed::s_model = NULL;

int Seaweed::s_texLoc;
int Seaweed::s_alphaModLoc;

Seaweed::Seaweed(Terrain* terrain)
{
	if (!s_shader)
	{
		s_shader = new Shader("seaweed_vert", "seaweed_frag");
		s_model = new Model("seaweed");
		s_texLoc = s_shader->uniformLocation("seaweedTex");
		s_alphaModLoc = s_shader->uniformLocation("alphaMod");
	}
	
	float x = float(rand()) / RAND_MAX * float(terrain->width()-1);
	float z = float(rand()) / RAND_MAX * float(terrain->height()-1);
	float y = terrain->heightAt(Vec2(x, z));
	
	m_pos = Vec3(x, y, z);
	m_rot = float(rand()) / RAND_MAX * 360.0;
	m_size = 0.25 + float(rand()) / RAND_MAX * 1.75;
}

Seaweed::~Seaweed()
{
}

void Seaweed::bind(float alphaMod)
{
	s_shader->bind();
	glUniform1f(s_alphaModLoc, alphaMod);
	
	glEnable(GL_BLEND);
}

void Seaweed::draw()
{
	glPushMatrix();
		glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
		glScalef(0.1 * m_size, 0.1 * m_size, 0.1 * m_size);
		glRotatef(m_rot, 0.0, 1.0, 0.0);
		s_model->draw(s_texLoc);
	glPopMatrix();
}

void Seaweed::unbind()
{
	s_shader->unbind();
	
	glDisable(GL_BLEND);
}


