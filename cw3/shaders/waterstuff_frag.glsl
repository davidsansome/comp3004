uniform sampler2D glowTex;

void main()
{
	vec4 sample = texture2D(glowTex, gl_TexCoord[0].xy);
	float size = gl_TexCoord[0].z;
	
	gl_FragColor = vec4(1.0, 1.0, 1.0, sample.r * 2.0);
}