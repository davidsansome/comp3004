#include "waterstuff.h"
#include "shader.h"
#include "texture.h"
#include "terrain.h"

WaterStuff::WaterStuff()
{
	m_glowTexture = Texture::load("glow.jpg");
	
	m_shader = new Shader("waterstuff_vert", "waterstuff_frag");
	m_glowTexLoc = m_shader->uniformLocation("glowTex");
}

WaterStuff::~WaterStuff()
{
	delete[] m_vertexBuffer;
	delete[] m_texCoordBuffer;
}

void WaterStuff::generate(int count, Terrain* terrain, float height)
{
	m_count = count;
	m_height = height;
	
	m_vertexBuffer = new float[m_count * 3 * 4];
	m_texCoordBuffer = new float[m_count * 4 * 4];
	float* v = m_vertexBuffer;
	float* t = m_texCoordBuffer;
	
	Blob blob;
	for (int i=0 ; i<count ; ++i)
	{
		// Random position
		blob.x = float(rand()) * terrain->width() / RAND_MAX;
		blob.z = float(rand()) * terrain->height() / RAND_MAX;
		
		// Don't make particles below the terrain
		float minHeight = terrain->heightAt(int(blob.x), int(blob.z));
		blob.y = minHeight + float(rand()) * (height - minHeight) / RAND_MAX;
		
		// Starting offset
		blob.offset = float(rand()) * M_PI * 2.0 / RAND_MAX;
		
		// Size
		blob.size = 0.025 + float(rand()) * 0.05 / RAND_MAX;
		
		doVertex(blob, &v, &t, 0.0, 0.0);
		doVertex(blob, &v, &t, 0.0, 1.0);
		doVertex(blob, &v, &t, 1.0, 1.0);
		doVertex(blob, &v, &t, 1.0, 0.0);
	}
}

void WaterStuff::doVertex(const Blob& blob, float** v, float** t, float dx, float dy)
{
	(*v)[0] = blob.x;
	(*v)[1] = blob.y;
	(*v)[2] = blob.z;
	
	(*t)[0] = dx;
	(*t)[1] = dy;
	(*t)[2] = blob.offset;
	(*t)[3] = blob.size;
	
	(*v) += 3;
	(*t) += 4;
}

void WaterStuff::bind()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, m_vertexBuffer);
	glTexCoordPointer(4, GL_FLOAT, 0, m_texCoordBuffer);
	
	glEnable(GL_BLEND);
	glDepthMask(false);
	
	m_shader->bind();
	
	m_glowTexture->bind(0, m_glowTexLoc);
}

void WaterStuff::unbind()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_BLEND);
	glDepthMask(true);

	m_shader->unbind();
}

void WaterStuff::draw()
{
	glDrawArrays(GL_QUADS, 0, m_count * 4);
}

void WaterStuff::update(float timeDelta)
{
	float* ver = m_vertexBuffer;
	float* tex = m_texCoordBuffer;
	
	for (int i=0 ; i<m_count ; ++i)
	{
		float y = ver[1];
		float size = tex[3];
		
		y += (0.1 + (0.075 - size) * 10.0) * timeDelta;
		
		if (y > m_height)
			y = 0.0;
		
		for (int e=0 ; e<4 ; ++e)
		{
			ver[1] = y;
			ver += 3;
		}
		tex += 8; // 2*4
	}
}
