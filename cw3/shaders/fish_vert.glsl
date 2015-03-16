#version 110

const float PI = 3.14159265358979323846;

uniform float t;
uniform float rotationMod;
uniform vec2 terrainSize;

mat4 rotationY(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	
	mat4 r = mat4
	(
		c,   0.0, s,   0.0,
		0.0, 1.0, 0.0, 0.0,
		-s,  0.0, c,   0.0,
		0.0, 0.0, 0.0, 1.0
	);
	
	return r;
}

mat3 normalRotationY(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	
	mat3 r = mat3
	(
		c,   0.0, -s,
		0.0, 1.0, 0.0,
		s,   0.0, c
	);
	
	return r;
}

void main()
{
	vec3 lightPos = gl_LightSource[0].position.xyz;
	
	// Do swimming animation
	float angle = rotationMod * sin(t) * pow(gl_Vertex.x - 0.5, 2.0) / 30.0;
	
	// Setup rotation matrices
	mat4 matrix = mat4(1.0);
	mat3 normalMatrix = mat3(1.0);
	matrix = rotationY(angle) * matrix;
	normalMatrix = normalRotationY(angle) * normalMatrix;
	
	vec4 pos = gl_ModelViewMatrix * (matrix * gl_Vertex);
	vec3 normal = normalize(gl_NormalMatrix * (normalMatrix * gl_Normal));
	
	// Calculate lighting
	vec3 lightVector = normalize(lightPos - pos.xyz);
	
	float ambient = 0.2;
	float diffuse = 0.7 * clamp(dot(lightVector, normal), 0.0, 1.0);
	
	// Distance from eye to point
	float dist = clamp(length(pos.xyz) / (terrainSize.y), 0.0, 1.0) * 0.3;
	
	gl_Position = gl_ProjectionMatrix * pos;
	gl_FrontColor = vec4(gl_Color.rgb * (ambient + diffuse), dist);
}