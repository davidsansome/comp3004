#version 110

uniform vec2 terrainSize;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec4 posEye = gl_ModelViewMatrix * gl_Vertex;
	
	// Distance from eye to point
	float distThreshold = max(0.0, length(gl_ModelViewMatrix * (vec4(terrainSize.x/2.0, 0.0, terrainSize.y/2.0, 1.0))) - terrainSize.y / 2.0);
	float dist = clamp((length(posEye.xyz) - distThreshold) / (terrainSize.y), 0.0, 1.0) * 0.5;
	
	gl_TexCoord[0] = vec4(gl_MultiTexCoord0.xy, dist, 0.0);
}