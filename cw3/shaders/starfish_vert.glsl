uniform vec3 lightPos;
uniform mat4 matrix;
uniform float t;

const vec2 terrainSize = vec2(80, 80);
const float frequency = 0.5;
const float amplitudeMod = 0.75;

void main()
{
	vec4 pos = gl_Vertex;
	
	// Animation
	// Convert coords to polar
	float radius = length(pos.xz);
	float theta = atan(pos.z, pos.x);
	
	// Calculate an amount to move the ends based on radius
	float amplitude = pow(radius / 2.0, 2.0);
	
	// Alter the polar coords
	theta += amplitude * amplitudeMod * sin(t * frequency);
	
	// Reconstruct the cartesian coords
	pos.x = radius * cos(theta);
	pos.z = radius * sin(theta);
	
	
	// Convert things to eye space
	vec3 posEye = (gl_ModelViewMatrix * pos).xyz;
	vec3 lightEye = (matrix * vec4(lightPos, 1.0)).xyz;
	vec3 normalEye = normalize(gl_NormalMatrix * gl_Normal);
	
	// Calculate vector from light to pos
	vec3 lightVector = normalize(lightEye - posEye);
	
	float ambient = 0.2;
	float diffuse = 0.7 * clamp(dot(lightVector, normalEye), 0.0, 1.0);
	float lighting = ambient + diffuse;
	
	// Distance from eye to point
	float dist = clamp(length(posEye.xyz) / (terrainSize.y), 0.0, 1.0) * 0.3;
	
	gl_Position = gl_ModelViewProjectionMatrix * pos;
	gl_TexCoord[0] = vec4(gl_MultiTexCoord0.xy, lighting, dist);
}