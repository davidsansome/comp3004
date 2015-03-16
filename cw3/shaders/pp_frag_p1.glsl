uniform sampler2D glow;

void main()
{
	vec3 sample = texture2D(glow, gl_TexCoord[0].xy).rgb +
	              texture2D(glow, gl_TexCoord[1].xy).rgb +
	              texture2D(glow, gl_TexCoord[2].xy).rgb +
	              texture2D(glow, gl_TexCoord[3].xy).rgb;
	
	sample /= 4.0;
	
	gl_FragColor = vec4(sample, 1.0);
}