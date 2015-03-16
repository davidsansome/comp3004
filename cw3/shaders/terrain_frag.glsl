uniform sampler2D sandTex;
uniform sampler2D mossTex;
uniform sampler2D caustic;

const vec3 blue = vec3(0.0, 0.5, 1.0);

const float zNear = 0.1;
const float zFar = 200.0;

float convertZ(float depthBufferValue )
{
	float clipZ = ( depthBufferValue - 0.5 ) * 2.0;
	return -(2.0 * zFar * zNear) / ( clipZ * ( zFar - zNear ) - ( zFar + zNear ));
}

void main()
{
	// Sample textures
	vec3 sandSample = texture2D(sandTex, gl_TexCoord[0].xy).rgb;
	vec3 mossSample = texture2D(mossTex, gl_TexCoord[0].xy).rgb;
	
	// Combine the textures according to weight calculated in vertex shader
	float weight = gl_TexCoord[0].z;
	vec3 sample = weight*mossSample + (1.0-weight)*sandSample;
	
	// Sample caustic
	float causticSample = texture2D(caustic, gl_TexCoord[1].yz * 0.1).r * 0.3;
	causticSample *= weight;
	
	// Calculate lighting
	float ambient = 0.2;
	float diffuse = gl_TexCoord[1].x;
	float lighting = clamp(ambient + diffuse, 0.0, 1.0);
	float attenuation = 0.0;
	float dist = gl_TexCoord[0].w;
	vec3 color = sample * lighting + causticSample;
	color = dist*blue + (1.0-dist)*color;
	
	gl_FragColor = vec4(color, 1.0-attenuation);
}