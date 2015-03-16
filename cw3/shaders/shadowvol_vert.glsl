void main()
{
	vec3 lightPos = gl_LightSource[0].position.xyz;
	
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;
	vec4 facePos = gl_ModelViewMatrix * vec4(gl_MultiTexCoord0.xyz, 1.0);
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
	
	// Calculate shadow volume stuff
	vec3 lightVector = normalize(lightPos - facePos.xyz);
	
	if (dot(lightVector, normal) < 0.0) // Point facing away from the light source
		pos.xyz -= normalize(lightPos - pos.xyz) * 20.0;
	
	gl_Position = gl_ProjectionMatrix * pos;
}