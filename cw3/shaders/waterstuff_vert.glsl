void main()
{
	// Unpack particle information
	vec2 texCoord = gl_MultiTexCoord0.xy;
	float startOffset = gl_MultiTexCoord0.z;
	float size = gl_MultiTexCoord0.w;
	
	// Transform the vertex position from object space to eye space
	vec4 eyePos = gl_ModelViewMatrix * gl_Vertex;
	
	// Offset coords
	eyePos.x += size * texCoord.x;
	eyePos.y += size * texCoord.y;
	
	// Output things
	gl_Position = gl_ProjectionMatrix * eyePos;
	gl_TexCoord[0] = vec4(texCoord, size, 0.0);
}