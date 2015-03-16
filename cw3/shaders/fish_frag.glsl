vec3 blue = vec3(0.0, 0.5, 1.0);

void main()
{
	vec3 color = gl_Color.rgb;
	float dist = gl_Color.a;
	
	// Make it blue when it's far away
	color = dist*blue + (1.0-dist)*color;
	
	gl_FragColor = vec4(color, 1.0);
}