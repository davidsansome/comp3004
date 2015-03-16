#include "tankwalls.h"
#include "terrain.h"
#include "shader.h"

TankWalls::TankWalls(Terrain* terrain, float bottom, float top)
	: m_terrain(terrain),
	  m_bottom(bottom),
	  m_top(top)
{
	m_shader = new Shader("tank_vert", "tank_frag");
	m_terrainSizeLoc = m_shader->uniformLocation("terrainSize");
	m_glassColorLoc = m_shader->uniformLocation("glassColor");
	m_distColorLoc = m_shader->uniformLocation("distColor");
	m_alphaModLoc = m_shader->uniformLocation("alphaMod");
}

TankWalls::~TankWalls()
{
	delete m_shader;
	
	glDeleteBuffers(3, m_bufferObjects);
}

void TankWalls::generate()
{
	m_horizCount = m_terrain->width()*2;
	m_vertCount = m_terrain->height()*2;
	int vertexCount = m_horizCount*2 + m_vertCount*2;
	
	float* vertexData = new float[vertexCount*3];
	float* texCoords = new float[vertexCount*2];
	float* normalData = new float[vertexCount*3];
	float* v = vertexData;
	float* t = texCoords;
	float* n = normalData;
	
	generateHorizEdge(&v, &t, &n, 0, -1.0, 1);
	generateVertEdge(&v, &t, &n, m_terrain->width()-1, -1.0, 1);
	generateHorizEdge(&v, &t, &n, m_terrain->height()-1, 1.0, -1);
	generateVertEdge(&v, &t, &n, 0, 1.0, -1);
	
	glGenBuffers(3, m_bufferObjects);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertexData, GL_STATIC_DRAW);
	delete[] vertexData;
	
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), texCoords, GL_STATIC_DRAW);
	delete[] texCoords;
	
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), normalData, GL_STATIC_DRAW);
	delete[] normalData;
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void TankWalls::generateHorizEdge(float** v, float** t, float** n, int y, float normalDirection, int incDirection)
{
	int start = incDirection>0 ? 0 : m_terrain->width()-1;
	for (int x=start ; x<m_terrain->width() && x>=0 ; x+=incDirection)
	{
		(*v)[0] = x;
		(*v)[1] = m_bottom;
		(*v)[2] = y;
		
		(*t)[0] = float(x) / TILE_SIZE;
		(*t)[1] = float(m_top - m_bottom) / TILE_SIZE;
		
		(*n)[0] = 0.0;
		(*n)[1] = 0.0;
		(*n)[2] = normalDirection;

		(*v)+=3; (*t)+=2; (*n)+=3;
		
		(*v)[0] = x;
		(*v)[1] = m_top;
		(*v)[2] = y;
		
		(*t)[0] = float(x) / TILE_SIZE;
		(*t)[1] = 0.0;
		
		(*n)[0] = 0.0;
		(*n)[1] = 0.0;
		(*n)[2] = normalDirection;

		(*v)+=3; (*t)+=2; (*n)+=3;
	}
}

void TankWalls::generateVertEdge(float** v, float** t, float** n, int x, float normalDirection, int incDirection)
{
	int start = incDirection>0 ? 0 : m_terrain->height()-1;
	for (int y=start ; y<m_terrain->height() && y>=0 ; y+=incDirection)
	{
		(*v)[0] = x;
		(*v)[1] = m_bottom;
		(*v)[2] = y;
		
		(*t)[0] = float(y) / TILE_SIZE;
		(*t)[1] = float(m_top - m_bottom) / TILE_SIZE;
		
		(*n)[0] = normalDirection;
		(*n)[1] = 0.0;
		(*n)[2] = 0.0;

		(*v)+=3; (*t)+=2; (*n)+=3;
		
		(*v)[0] = x;
		(*v)[1] = m_top;
		(*v)[2] = y;
		
		(*t)[0] = float(y) / TILE_SIZE;
		(*t)[1] = 0.0;
		
		(*n)[0] = normalDirection;
		(*n)[1] = 0.0;
		(*n)[2] = 0.0;

		(*v)+=3; (*t)+=2; (*n)+=3;
	}
}

void TankWalls::draw()
{
	// y=0
	glStencilFunc(GL_ALWAYS, 0x90, 0xF0);
	glDrawArrays(GL_QUAD_STRIP, 0, m_horizCount);
	
	// x=w
	glStencilFunc(GL_ALWAYS, 0xA0, 0xF0);
	glDrawArrays(GL_QUAD_STRIP, m_horizCount, m_vertCount);
	
	// y=h
	glStencilFunc(GL_ALWAYS, 0xB0, 0xF0);
	glDrawArrays(GL_QUAD_STRIP, m_horizCount + m_vertCount, m_horizCount);
	
	// x=0
	glStencilFunc(GL_ALWAYS, 0xC0, 0xF0);
	glDrawArrays(GL_QUAD_STRIP, m_horizCount*2 + m_vertCount, m_vertCount);
}

void TankWalls::bind(const Vec3& glassColor, const Vec3& distColor, float alphaMod)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[1]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObjects[2]);
	glNormalPointer(GL_FLOAT, 0, 0);
	
	glEnable(GL_BLEND);
	glDepthMask(false);
	glPolygonOffset(-1.0, 0.0);
	glEnable(GL_POLYGON_OFFSET_FILL);

	m_shader->bind();
	glUniform2f(m_terrainSizeLoc, m_terrain->width()-1, m_terrain->height()-1);
	glUniform3f(m_glassColorLoc, glassColor[0], glassColor[1], glassColor[2]);
	glUniform3f(m_distColorLoc, distColor[0], distColor[1], distColor[2]);
	glUniform1f(m_alphaModLoc, alphaMod);
}

void TankWalls::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	glDisable(GL_BLEND);
	glDepthMask(true);
	glDisable(GL_POLYGON_OFFSET_FILL);

	m_shader->unbind();
}


