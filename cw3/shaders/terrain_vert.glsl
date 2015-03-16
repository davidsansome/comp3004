#version 110

#define TILE_SIZE 4.0

const float PI = 3.14159265358979323846;
uniform vec3 lightVector;
const float fogStart = 50.0;
const float fogEnd = 80.0;
uniform vec2 terrainSize;

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
	gl_TexCoord[1] = vec4(diffuse, gl_Vertex.xz, 0.0);
	gl_TexCoord[2] = vec4(vec2(gl_Vertex.x, -gl_Vertex.z) / terrainSize, 0.0, 0.0);
}