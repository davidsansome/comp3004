const float baseAmplitude = 0.4;
const float coordFrequency = 10.0;
const float timeFrequency = 2.0;

const float E = 2.71828183;
const float PI = 3.14159265358979323846;

uniform sampler2D noise;

uniform float t;

void main()
{
	// Unpack stuff
	vec3 posEye = gl_TexCoord[0].xyz;
	vec3 lightEye = gl_TexCoord[1].xyz;
	vec3 normalEye = normalize(gl_TexCoord[2].xyz);
	vec2 surfacePos = gl_TexCoord[3].xy;
	
	// Calculate vector from light to pos
	vec3 lightVector = normalize(lightEye - posEye);
	
	// Sample some noise
	vec3 noise = texture2D(noise, surfacePos * 1.5 + t * 0.001).rgb;
	
	// Modify our texcoords by some amount
	vec3 wobblyNormal = normalEye;
	wobblyNormal.x += baseAmplitude * noise.r * sin(coordFrequency * (surfacePos.x + surfacePos.y + noise.g) + timeFrequency * t);
	wobblyNormal.z += baseAmplitude * noise.r * sin(coordFrequency * (surfacePos.x + surfacePos.y + noise.b) + timeFrequency * t);
	wobblyNormal = normalize(wobblyNormal);
	
	// Calculate viewing angle
	vec3 viewingVec = normalize(posEye);
	float viewingAngle = 1.0 - pow(dot(normalEye, viewingVec), 2.0);
	
	// Calculate lighting
	float ambient = 0.2;
	float diffuse = 0.5 * pow(dot(lightVector, wobblyNormal), 4.0);
	float specular = clamp(pow(max(dot(reflect(lightVector, wobblyNormal), viewingVec), 0.0), 20.0), 0.0, 1.0);
	float lighting = min(1.0, ambient + diffuse + specular);
	
	// Start at an off-blue color and mix it with white light
	vec3 color = vec3(0.2, 0.4, 0.6);
	color = 0.8 * (1.0 - lighting)*color + lighting;
	
	// Alpha is based on intensity of light and viewing angle
	float alpha = 0.5 + viewingAngle * lighting * 0.5;
	
	gl_FragColor = vec4(color, alpha);
}