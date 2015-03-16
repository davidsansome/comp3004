#version 110

uniform vec3 lightPos;

void main()
{
	// Convert things to eye space
	vec4 posEye = gl_ModelViewMatrix * gl_Vertex;
	
	// Output the position
	gl_Position = gl_ProjectionMatrix * posEye;
	
	// Pass stuff to the fragment shader
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_Vertex;
	gl_TexCoord[2] = vec4(lightPos, 1.0);
}