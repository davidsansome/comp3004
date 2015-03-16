#version 110

uniform vec3 lightPos;

void main()
{
	// Convert things to eye space
	vec4 posEye = gl_ModelViewMatrix * gl_Vertex;
	vec4 lightEye = gl_ModelViewMatrix * vec4(lightPos, 1.0);
	vec4 normalEye = vec4(gl_NormalMatrix * vec3(0.0, 1.0, 0.0), 1.0);
	
	// Output the position
	gl_Position = gl_ProjectionMatrix * posEye;
	
	// Pass stuff to the fragment shader
	gl_TexCoord[0] = posEye;
	gl_TexCoord[1] = lightEye;
	gl_TexCoord[2] = normalEye;
	gl_TexCoord[3] = gl_MultiTexCoord0;
}