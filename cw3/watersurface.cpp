#include "watersurface.h"
#include "terrain.h"
#include "shader.h"
#include "texture.h"

#define TESS 10

WaterSurface::WaterSurface(Terrain* terrain, float height)
	: m_terrain(terrain),
	  m_height(height)
{
	m_shader = new Shader("watersurface_vert", "watersurface_frag");
	m_tLoc = m_shader->uniformLocation("t");
	m_lightPosLoc = m_shader->uniformLocation("lightPos");
	m_noiseTexLoc = m_shader->uniformLocation("noise");
	
	m_noiseTex = Texture::load("noise.jpg");
}

WaterSurface::~WaterSurface()
{
	glDeleteBuffers(2, m_bufferObjects);
}

void WaterSurface::finalize()
{
	m_elements = TESS*2*(TESS+1);
	float* vertexData = new float[m_elements * 3];
	float* texCoordData = new float[m_elements * 2];
	float* v = vertexData;
	float* t = texCoordData;
	
	int w = m_terrain->width() - 1;
	int h = m_terrain->height() - 1;
	
	for (int x=0 ; x<TESS ; ++x)
	{
		for (int y=0 ; y<=TESS ; ++y)
		{
			setVertex(&v, &t, float(x)/TESS, float(y)/TESS, w, h);
			setVertex(&v, &t, float(x+1)/TESS, float(y)/TESS, w, h);
		}
	}
	
	glGenBuffers(2, m_bufferObjects);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[0]);
	glBufferData(GL_ARRAY_BUFFER, m_elements * 3 * sizeof(float), vertexData, GL_STATIC_DRAW);
	delete[] vertexData;

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[1]);
	glBufferData(GL_ARRAY_BUFFER, m_elements * 2 * sizeof(float), texCoordData, GL_STATIC_DRAW);
	delete[] texCoordData;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void WaterSurface::setVertex(float** v, float** t, float tx, float ty, int w, int h)
{
	(*v)[0] = tx * w;
	(*v)[1] = m_height;
	(*v)[2] = ty * h;
	
	(*t)[0] = tx;
	(*t)[1] = ty;
	
	(*v) += 3;
	(*t) += 2;
}

void WaterSurface::setParams(float t, const Vec3& lightPos)
{
	m_t = t;
	m_lightPos = lightPos;
}

void WaterSurface::bind()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[1]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	
	m_shader->bind();
	glUniform1f(m_tLoc, m_t);
	glUniform3f(m_lightPosLoc, m_lightPos[0], m_lightPos[1], m_lightPos[2]);
	
	m_noiseTex->bind(0, m_noiseTexLoc);
	
	glEnable(GL_BLEND);
}

void WaterSurface::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	m_shader->unbind();
	
	glDisable(GL_BLEND);
}

void WaterSurface::draw()
{
	int stripSize = 2*(TESS+1);
	for (int i=0 ; i<TESS ; ++i)
		glDrawArrays(GL_TRIANGLE_STRIP, i*stripSize, stripSize);
}


