uniform sampler2D glow;

void main()
{
	vec3 sample = texture2D(glow, gl_TexCoord[0].xy).rgb + 0.05;
	sample = clamp(sample * sample * sample, 0.0, 1.0);
	
	gl_FragColor = vec4(sample, 1.0);
}