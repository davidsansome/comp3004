uniform sampler2D seaweedTex;
uniform float alphaMod;

void main()
{
	vec4 color = texture2D(seaweedTex, gl_TexCoord[0].xy);
	color.a = 1.0 - (color.r + color.g + color.b) / 3.0;
	color.a *= alphaMod;
	
	gl_FragColor = color;
}