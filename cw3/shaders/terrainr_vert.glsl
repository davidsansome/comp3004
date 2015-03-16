#version 110

#define TILE_SIZE 4.0

const float PI = 3.14159265358979323846;
uniform vec3 lightVector;
const float fogStart = 50.0;
const float fogEnd = 80.0;
uniform vec2 terrainSize;

float attenuation()
{
	// Calculate how far away from the edge we are
	float attenuation = 0.0;
	if (gl_Vertex.x < 0.0)
		attenuation = abs(gl_Vertex.x) / TILE_SIZE;
	else if (gl_Vertex.z < 0.0)
		attenuation = abs(gl_Vertex.z) / TILE_SIZE;
	else if (gl_Vertex.x > terrainSize.x)
		attenuation = (gl_Vertex.x - terrainSize.x) / TILE_SIZE;
	else if (gl_Vertex.z > terrainSize.y)
		attenuation = (gl_Vertex.z - terrainSize.y) / TILE_SIZE;
	
	return attenuation;
}

vec2 causticCoords()
{
	vec2 causticCoords = gl_Vertex.xz;
	
	// Calculate how far away from the edge we are
	if (gl_Vertex.x < 0.0)
		causticCoords.x = -causticCoords.x;
	else if (gl_Vertex.z < 0.0)
		causticCoords.y = -causticCoords.y;
	
	return causticCoords;
}

void main()
{
	// Transform the vertex position from object space to clip space
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec4 posEye = gl_ModelViewMatrix * gl_Vertex;
	
	// Transform things into view space
	vec3 normalViewSpace = normalize(gl_ModelViewMatrix * vec4(gl_Normal, 0.0)).xyz;
	vec3 lightViewSpace = normalize(gl_ModelViewMatrix * vec4(lightVector, 0.0)).xyz;
	
	// Calculate diffuse lighting
	float diffuse = 0.7 * max(dot(lightViewSpace, normalViewSpace), 0.0);

	// Calculate the density of the grass
	float mossVal = pow(gl_Normal.y, 200.0);
	
	// Distance from eye to point
	float distThreshold = max(0.0, length(gl_ModelViewMatrix * (vec4(terrainSize.x/2.0, 0.0, terrainSize.y/2.0, 1.0))) - terrainSize.y / 2.0);
	float dist = clamp((length(posEye.xyz) - distThreshold) / (terrainSize.y / 2.0), 0.0, 1.0) * 0.3;

	// Output values to the fragment shader
	gl_TexCoord[0] = vec4(gl_MultiTexCoord0.xy, mossVal, dist);
	gl_TexCoord[1] = vec4(diffuse, causticCoords(), attenuation());
}