#version 110

uniform vec2 offset;

void main()
{
	// Passthrough position
	gl_Position = gl_Vertex;
	gl_TexCoord[0] = vec4(gl_MultiTexCoord0.xy + offset,       0.0, 0.0);
	gl_TexCoord[1] = vec4(gl_MultiTexCoord0.xy + offset * 2.0, 0.0, 0.0);
	gl_TexCoord[2] = vec4(gl_MultiTexCoord0.xy - offset,       0.0, 0.0);
	gl_TexCoord[3] = vec4(gl_MultiTexCoord0.xy - offset * 2.0, 0.0, 0.0);
}