uniform sampler2D glassTex;
uniform vec3 glassColor;
uniform vec3 distColor;
uniform float alphaMod;

void main()
{
	float dist = gl_TexCoord[0].z;
	
	vec3 color = dist*distColor + (1.0-dist)*glassColor;
	float alpha = min(1.0, (0.25 + dist) * alphaMod);
	
	gl_FragColor = vec4(color, alpha);
}