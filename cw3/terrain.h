/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 *                                                                         *
 * This file was taken from Baarbarity, (c) David Sansome 2006, and        *
 * modified for COMP3004.                                                  *
 * http://fluffy.ecs.soton.ac.uk/~ds505/svn/baarbarity/                    *
 *                                                                         *
 ***************************************************************************/

#ifndef TERRAIN_H
#define TERRAIN_H

#define TILE_SIZE 4
#define TILE_SIZE_PLUS_1 (TILE_SIZE+1)

#define ALPHA_MAP_SIZE_MUL 2

#define MAX_HEIGHT 5.0
#define SAND_DEPTH 2.0

#include <svl/SVL.h>

class Shader;
class Texture;

class Terrain
{
public:
	Terrain(int xTiles, int yTiles);
	Terrain();
	~Terrain();
	
	void finalize();
	
	void setParams(const Vec3& lightVector);

	void bind();
	void unbind();
	void bindReflections();

	void generate(int iterations, int minDistance, int maxDistance, float flattenFactor);

	int width() { return m_width; }
	int height() { return m_height; }
	int xTiles() { return m_xTiles; }
	int yTiles() { return m_yTiles; }

	float heightAt(int x, int y) const;
	float heightAt(const Vec2& pos) const;
	Vec3 normalAt(int x, int y) const;
	Vec3 normalAt(const Vec2& pos) const;

	void setCaustic(int caustic) { m_caustic = caustic; }
	void draw();
	void drawReflections();
	
private:
	void setHeightAt(int x, int y, float value);
	void setNormalAt(int x, int y, float* value);
	
	void calculateVertexNormal(int x, int y);
	void calculatePlaneNormal(int x, int y, int x1, int y1, int x2, int y2, float* result);

	void normalize();
	void flatten(float power);
	void calculateNormals();
	void addVertexToBuffer(float* v, float* n, float* t, int x, int y);
	void addVertexToBufferMirrored(float* v, float* n, float* t, int x, int y, int realX, int realY);
	void finalizeHorizEdge(float** v, float** n, float** t, int y, float normalDirection, int incDirection);
	void finalizeVertEdge(float** v, float** n, float** t, int x, float normalDirection, int incDirection);
	
	void drawStrip(int strip);
	void drawEdges();
	void drawReflection(int* first, int count);
	
	void bindGeneral();
	void unbindGeneral();

private:
	int m_xTiles;
	int m_yTiles;
	int m_width;
	int m_height;
	
	int m_caustic;

	float* m_heightMap;
	float* m_normalMap;
	uint m_bufferObjects[3];
	
	Shader* m_shader;
	Shader* m_shaderr;
	Texture* m_sandTexture;
	Texture* m_mossTexture;
	Texture* m_caustTextures[32];
	
	int m_sandTexLoc;
	int m_mossTexLoc;
	int m_caustTexLoc;
	int m_terrainSizeLoc;
	int m_sandTexLocR;
	int m_mossTexLocR;
	int m_caustTexLocR;
	int m_terrainSizeLocR;
	int m_lightVectorLoc;
	int m_lightVectorLocR;
	
	Vec3 m_lightVector;
};

#endif
