#ifndef MODEL_H
#define MODEL_H

#include "utilities.h"

#include <list>
#include <vector>
#include <istream>

class Texture;

class Mesh
{
public:
	Mesh() : m_boundingRadius(0.0) {}
	virtual ~Mesh();
	virtual void load(istream& stream) = 0;
	virtual void draw(int shaderLoc) = 0;
	void drawShadowVolume();
	
	float boundingRadius() const { return m_boundingRadius; }

protected:
	void loadEdges(istream& stream);
	
	float m_boundingRadius;
	float* m_edgeVertices;
	float* m_edgeNormals;
	float* m_edgeCenters;
	int m_edgeCount;
};

class ColoredMesh : public Mesh
{
public:
	void load(istream& stream);
	void draw(int shaderLoc);

private:
	int m_elements;
	float* m_vertices;
	float* m_colors;
	float* m_normals;
};

class VertexGroup
{
public:
	VertexGroup(const string& textureName);
	~VertexGroup();
	
	void addVertex(const Vec& v, const Vec& n, const Vec& t);
	void finalize();
	void draw(int shaderLoc);

private:
	Texture* m_texture;
	
	list<float> m_vertexArray;
	list<float> m_normalArray;
	list<float> m_texCoordArray;
	
	int m_elements;
	float* m_vertices;
	float* m_normals;
	float* m_texCoords;
};

class TexturedMesh : public Mesh
{
public:
	~TexturedMesh();
	void load(istream& stream);
	void draw(int shaderLoc);

private:
	vector<VertexGroup*> m_groups;
};


class Model
{
public:
	Model(const string& filename);
	~Model();

	void draw(int shaderLoc = -1);
	void drawShadowVolume();
	float boundingRadius() const { return m_boundingRadius; }

private:
	void load(const string& filename);
	
	vector<Mesh*> m_meshes;
	float m_boundingRadius;
};

#endif
