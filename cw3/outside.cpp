#include "outside.h"
#include "texture.h"
#include "shader.h"
#include "terrain.h"

Outside::Outside(Terrain* terrain)
	: m_terrain(terrain)
{
	m_shader = new Shader("table_vert", "table_frag");
	m_woodTexture = Texture::load("wood.jpg");
	
	m_woodTextureLoc = m_shader->uniformLocation("woodTex");
	m_lightPosLoc = m_shader->uniformLocation("lightPos");
}

Outside::~Outside()
{
	delete m_shader;
	delete m_woodTexture;
}

void Outside::setParams(const Vec3& lightPos)
{
	m_lightPos = lightPos;
}

void Outside::bind()
{
	m_shader->bind();
	glUniform3f(m_lightPosLoc, m_lightPos[0], m_lightPos[1], m_lightPos[2]);
	
	m_woodTexture->bind(0, m_woodTextureLoc);
}

void Outside::unbind()
{
	m_shader->unbind();
}

void Outside::draw()
{
	float w = m_terrain->width()-1;
	float h = m_terrain->height()-1;
	float y = -SAND_DEPTH;
	float e = 2.0;
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-w*e, y, -h*e);

		glTexCoord2f(1.0, 0.0);
		glVertex3f(w*(e+1.0), y, -h*e);
		
		glTexCoord2f(1.0, 1.0);
		glVertex3f(w*(e+1.0), y, h*(e+1.0));
		
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-w*e, y, h*(e+1.0));
	glEnd();
}


