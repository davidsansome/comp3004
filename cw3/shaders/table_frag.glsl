const float PI = 3.14159265358979323846;

uniform sampler2D woodTex;

void main()
{
	vec2 tableCoords = gl_TexCoord[0].xy;
	vec2 texCoords = tableCoords * 6.0;
	
	// Unpack stuff
	vec3 posEye = gl_TexCoord[1].xyz;
	vec3 lightEye = gl_TexCoord[2].xyz;
	
	// Calculate lighting
	vec3 lightVector = normalize(lightEye - posEye);
	vec3 normal = vec3(0.0, 1.0, 0.0);
	
	float intensity = pow(max(0.0, dot(lightVector, normal)), 2.5);
	
	gl_FragColor = vec4(texture2D(woodTex, texCoords).xyz * intensity, 1.0);
}