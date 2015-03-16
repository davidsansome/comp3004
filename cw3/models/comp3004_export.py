#!BPY

"""
Name: 'COMP3004 Model Exporter (.mdl)...'
Blender: 242
Group: 'Export'
Tooltip: 'Export selected mesh to COMP3004 model format (.mdl)'
"""

__author__ = "David Sansome"

import Blender
import BPyMesh
import os
from struct import pack

def write2(file, v):
	file.write(pack("ff", v[0], 1.0-v[1]))

# Notice y and z are flipped
def write3(file, v):
	file.write(pack("fff", v[0], v[2], v[1]))

def writeCoord(file, mesh, face, i):
	write3(file, face[0].verts[face[i]].co) # Coords
	if face[0].smooth:
		write3(file, face[0].verts[face[i]].no) # Normal
	else:
		write3(file, face[0].no) # Normal
	if mesh.faceUV:
		write2(file, face[0].uv[face[i]]) # UV coords

def writeMesh(file, scene, o):
	mesh = BPyMesh.getMeshFromObject(o, None, True, False, scene)
	mesh.transform(o.matrixWorld)
	
	print "%i + %i - %i = %i\n" % (len(mesh.verts), len(mesh.faces), len(mesh.edges), len(mesh.verts) + len(mesh.faces) - len(mesh.edges))
	
	if mesh.faceUV:
		# Count up the textures
		textures = []
		for f in mesh.faces:
			if f.image == None:
				raise Exception("UV face without texture filename.  Coords of first vertex are %f,%f,%f" % (f.verts[0].co[0], f.verts[0].co[1], f.verts[0].co[2]))
			filename = os.path.basename(f.image.filename)
			if not filename in textures:
				textures.append(filename)
		
		file.write(pack("BB", 1, len(textures)))
		for texture in textures:
			file.write(pack("B", len(texture)))
			file.write(texture)
	else:
		i=0
		file.write(pack("BB", 0, len(mesh.materials)))
		for mat in mesh.materials:
			file.write(pack("fff", mat.R, mat.G, mat.B))
			i=i+1
	
	faces = []
	for f in mesh.faces:
		if len(f.verts) == 3:
			faces.append([f, 0, 1, 2])
		elif len(f.verts) == 4:
			faces.append([f, 0, 1, 2])
			faces.append([f, 0, 2, 3])
		else:
			raise Exception("Invalid number of vertices %i" % len(f.verts))
	
	# Build a list of all the edges in the model
	edgesTemp = []
	for e in mesh.edges:
		edgesTemp.append([e.v1.co, e.v2.co, None, None, None, None])
	
	# Now find face normals that correspond to those edges
	for f in faces:
		for i in (0, 1, 2):
			v1 = f[0].verts[f[i + 1]].co
			v2 = f[0].verts[f[(i + 1) % 3 + 1]].co
			
			for e in edgesTemp:
				if e[0] == v1 and e[1] == v2 and e[2] == None:
					e[2] = f[0].no
					e[4] = f[0].cent
					break
				elif e[0] == v2 and e[1] == v1 and e[3] == None:
					e[3] = f[0].no
					e[5] = f[0].cent
					break
				elif e[0] == v1 and e[1] == v2 or e[1] == v1 and e[0] == v2:
					print "Edge with more than 2 faces.  Wtf?"
	
	# Keep only edges with exactly 2 faces connected to them
	edges = []
	for e in edgesTemp:
		if e[2] != None and e[3] != None:
			edges.append(e)
		else:
			print "Edge with < 2 faces.  Fool"
	
	# Write out faces
	file.write(pack("I", len(faces)))
	for face in faces:
		if mesh.faceUV:
			file.write(pack("B", textures.index(os.path.basename(face[0].image.filename))))
		else:
			file.write(pack("B", face[0].mat)) # Material
		
		writeCoord(file, mesh, face, 1)
		writeCoord(file, mesh, face, 2)
		writeCoord(file, mesh, face, 3)
	
	# Write out edges
	file.write(pack("I", len(edges)))
	for e in edges:
		for i in range(0, 6):
			write3(file, e[i])

def write(filename):
	if not filename.lower().endswith('.mdl'):
		filename += '.mdl'
	
	scene = Blender.Scene.GetCurrent()
	
	meshes = []
	for o in scene.objects:
		if o.type == 'Mesh':
			meshes.append(o)
	
	file = open(filename, 'wb')
	file.write(pack("I", len(meshes)))
	for mesh in meshes:
		writeMesh(file, scene, mesh)
	file.close()


def main():
	Blender.Window.FileSelector(write, 'COMP3004 Export', Blender.sys.makename(ext='.mdl'))


if __name__=='__main__':
	main()
