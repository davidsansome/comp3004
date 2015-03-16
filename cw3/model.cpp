#include "model.h"
#include "texture.h"

#include <fstream>
#include <vector>
#include <svl/SVL.h>

#define SETVEC(dest, i, vec) dest[i + 0] = vec[0]; \
                             dest[i + 1] = vec[1]; \
                             dest[i + 2] = vec[2];

void Mesh::loadEdges(istream& stream)
{
	using Utilities::read;
	using Utilities::readVec;
	
	uint count = read<uint>(stream);
	m_edgeCount = count * 4;
	m_edgeVertices = new float[count * 4 * 3];
	m_edgeNormals = new float[count * 4 * 3];
	m_edgeCenters = new float[count * 4 * 3];
	
	for (uint i=0 ; i<count ; ++i)
	{
		Vec v1 = readVec<3>(stream);
		Vec v2 = readVec<3>(stream);
		Vec n1 = readVec<3>(stream);
		Vec n2 = readVec<3>(stream);
		Vec c1 = readVec<3>(stream);
		Vec c2 = readVec<3>(stream);
		
		SETVEC(m_edgeVertices, i*12 + 0, v1)
		SETVEC(m_edgeVertices, i*12 + 3, v2)
		SETVEC(m_edgeVertices, i*12 + 6, v2)
		SETVEC(m_edgeVertices, i*12 + 9, v1)
		
		SETVEC(m_edgeNormals, i*12 + 0, n1)
		SETVEC(m_edgeNormals, i*12 + 3, n1)
		SETVEC(m_edgeNormals, i*12 + 6, n2)
		SETVEC(m_edgeNormals, i*12 + 9, n2)
		
		SETVEC(m_edgeCenters, i*12 + 0, c1)
		SETVEC(m_edgeCenters, i*12 + 3, c1)
		SETVEC(m_edgeCenters, i*12 + 6, c2)
		SETVEC(m_edgeCenters, i*12 + 9, c2)
	}
}

void Mesh::drawShadowVolume()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, m_edgeVertices);
	glNormalPointer(GL_FLOAT, 0, m_edgeNormals);
	glTexCoordPointer(3, GL_FLOAT, 0, m_edgeCenters);
	
	glDrawArrays(GL_QUADS, 0, m_edgeCount);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

Mesh::~Mesh()
{
	delete[] m_edgeVertices;
	delete[] m_edgeNormals;
	delete[] m_edgeCenters;
}

VertexGroup::VertexGroup(const string& textureName)
{
	m_texture = Texture::load(textureName);
}

VertexGroup::~VertexGroup()
{
	delete[] m_vertices;
	delete[] m_normals;
	delete[] m_texCoords;
}

void VertexGroup::addVertex(const Vec& v, const Vec& n, const Vec& t)
{
	m_vertexArray.push_back(v[0]);
	m_vertexArray.push_back(v[1]);
	m_vertexArray.push_back(v[2]);
	m_normalArray.push_back(n[0]);
	m_normalArray.push_back(n[1]);
	m_normalArray.push_back(n[2]);
	m_texCoordArray.push_back(t[0]);
	m_texCoordArray.push_back(t[1]);
}

void VertexGroup::finalize()
{
	// Copy them to float arrays
	m_elements = m_vertexArray.size() / 3;
	m_vertices = new float[m_vertexArray.size()];
	m_normals = new float[m_normalArray.size()];
	m_texCoords = new float[m_texCoordArray.size()];
	
	copy(m_vertexArray.begin(), m_vertexArray.end(), m_vertices);
	copy(m_normalArray.begin(), m_normalArray.end(), m_normals);
	copy(m_texCoordArray.begin(), m_texCoordArray.end(), m_texCoords);
	
	m_vertexArray.clear();
	m_normalArray.clear();
	m_texCoordArray.clear();
}

void VertexGroup::draw(int shaderLoc)
{
	m_texture->bind(0, shaderLoc);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, m_vertices);
	glNormalPointer(GL_FLOAT, 0, m_normals);
	glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords);
	
	glDrawArrays(GL_TRIANGLES, 0, m_elements);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}





void ColoredMesh::load(istream& stream)
{
	using Utilities::read;
	using Utilities::readVec;
	
	list<float> vertices;
	list<float> colors;
	list<float> normals;
	
	// Read material colors
	uint8 matCount = read<uint8>(stream);
	vector<Vec3> materials(matCount);
	for (uint mat=0 ; mat<matCount ; ++mat)
		materials[mat] = Vec3(read<float>(stream), read<float>(stream), read<float>(stream));
	
	// Read faces
	uint faceCount = read<uint>(stream);
	for (uint face=0 ; face<faceCount ; ++face)
	{
		// Read the face material
		uint8 mat = read<uint8>(stream);
		for (int j=0 ; j<3 ; ++j)
		{
			colors.push_back(materials[mat][0]);
			colors.push_back(materials[mat][1]);
			colors.push_back(materials[mat][2]);
		}
		
		// Read the 3 vertices
		for (int j=0 ; j<3 ; ++j)
		{
			Vec vertex = readVec<3>(stream);
			m_boundingRadius = max(m_boundingRadius, float(len(vertex)));
			vertices.push_back(vertex[0]);
			vertices.push_back(vertex[1]);
			vertices.push_back(vertex[2]);
			
			normals.push_back(read<float>(stream));
			normals.push_back(read<float>(stream));
			normals.push_back(read<float>(stream));
		}
	}
	
	loadEdges(stream);
	
	// Copy them to float arrays
	m_elements = vertices.size() / 3;
	m_vertices = new float[vertices.size()];
	m_normals = new float[normals.size()];
	m_colors = new float[colors.size()];
	
	copy(vertices.begin(), vertices.end(), m_vertices);
	copy(normals.begin(), normals.end(), m_normals);
	copy(colors.begin(), colors.end(), m_colors);
}

void ColoredMesh::draw(int shaderLoc)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, m_vertices);
	glNormalPointer(GL_FLOAT, 0, m_normals);
	glColorPointer(3, GL_FLOAT, 0, m_colors);
	
	glDrawArrays(GL_TRIANGLES, 0, m_elements);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}





TexturedMesh::~TexturedMesh()
{
	for (vector<VertexGroup*>::iterator it = m_groups.begin() ; it != m_groups.end() ; it++)
		delete (*it);
}

void TexturedMesh::load(istream& stream)
{
	using Utilities::read;
	using Utilities::readVec;
	using Utilities::readString;
	
	list<float> vertices;
	list<float> normals;
	list<float> texCoords;
	
	// Read textures
	uint8 texCount = read<uint8>(stream);
	m_groups.reserve(texCount);
	for (uint tex=0 ; tex<texCount ; ++tex)
		m_groups.push_back(new VertexGroup(readString(stream)));
	
	// Read faces
	uint faceCount = read<uint>(stream);
	for (uint face=0 ; face<faceCount ; ++face)
	{
		// Read the face texture
		uint8 tex = read<uint8>(stream);
		
		// Read the 3 vertices
		for (int j=0 ; j<3 ; ++j)
		{
			Vec v = readVec<3>(stream);
			Vec n = readVec<3>(stream);
			Vec t = readVec<2>(stream);
			
			m_boundingRadius = max(m_boundingRadius, float(len(v)));
			
			m_groups[tex]->addVertex(v, n, t);
		}
	}
	
	loadEdges(stream);
	
	for (vector<VertexGroup*>::iterator it = m_groups.begin() ; it != m_groups.end() ; it++)
		(*it)->finalize();
}

void TexturedMesh::draw(int shaderLoc)
{
	for (vector<VertexGroup*>::iterator it = m_groups.begin() ; it != m_groups.end() ; it++)
		(*it)->draw(shaderLoc);
}






Model::Model(const string& filename)
	: m_boundingRadius(0.0)
{
	// Because GDB doesn't debug constructors properly and it was annoying me
	load(filename);
}

Model::~Model()
{
	for (vector<Mesh*>::iterator it = m_meshes.begin() ; it != m_meshes.end() ; it++)
		delete (*it);
}

void Model::load(const string& filename)
{
	using Utilities::read;
	
	ifstream stream((filename + ".mdl").c_str(), ios::in | ios::binary);
	if (!stream.is_open())
		Utilities::fatal("Could not open model " + filename);
	
	uint meshCount = read<uint>(stream);
	for (uint mesh=0 ; mesh<meshCount ; ++mesh)
	{
		Mesh* mesh;
		uint8 uv = read<uint8>(stream);
		
		if (uv == 1)
			mesh = new TexturedMesh();
		else
			mesh = new ColoredMesh();
		
		mesh->load(stream);
		m_meshes.push_back(mesh);
		
		m_boundingRadius = max(m_boundingRadius, mesh->boundingRadius());
	}
	
	cerr << "Loaded model: " << filename << ".mdl" << endl;
	cerr << " - bounding radius: " << m_boundingRadius << endl;
	cerr << " - meshes: " << meshCount << endl;
}

void Model::draw(int shaderLoc)
{
	for (vector<Mesh*>::iterator it = m_meshes.begin() ; it != m_meshes.end() ; it++)
		(*it)->draw(shaderLoc);
}

void Model::drawShadowVolume()
{
	for (vector<Mesh*>::iterator it = m_meshes.begin() ; it != m_meshes.end() ; it++)
		(*it)->drawShadowVolume();
}



