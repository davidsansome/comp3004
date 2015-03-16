uniform sampler2D texture;

const vec3 blue = vec3(0.0, 0.5, 1.0);

void main()
{
	// Unpack
	vec2 texCoord = gl_TexCoord[0].xy;
	float lighting = gl_TexCoord[0].z;
	float dist = gl_TexCoord[0].w;
	
	// Sample textures and apply lighting
	vec3 color = texture2D(texture, texCoord).rgb;
	color = color * lighting;
	
	// Make it blue when it's far away
	color = dist*blue + (1.0-dist)*color;
	
	gl_FragColor = vec4(color, 1.0);
}