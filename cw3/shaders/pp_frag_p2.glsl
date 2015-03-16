uniform sampler2D colorBuffer;
uniform sampler2D noise;
uniform sampler2D glow;
uniform float t;
uniform vec2 pixelSize;
uniform vec2 screenSize;
uniform vec2 viewportSize;

const float baseAmplitude = 0.0015;
const float coordFrequency = 5.0;
const float timeFrequency = 3.5;
const float noiseStretch = 1.0;
const float noiseFrequency = 0.0001;

void main()
{
	// Make the amplitude a function of screen size
	float amplitude = 1.0/screenSize.y * 1200.0 * baseAmplitude;
	
	// Sample some noise
	vec2 coord = gl_TexCoord[0].xy;
	vec3 noise = texture2D(noise, coord * noiseStretch + t * noiseFrequency).rgb;
	
	// Modify our texcoords by some amount
	coord.x += amplitude * noise.r * sin(coordFrequency * (coord.x + coord.y + noise.g) + timeFrequency * t);
	coord.y += amplitude * noise.r * sin(coordFrequency * (coord.x + coord.y + noise.b) + timeFrequency * t);
	
	// Sample our original image
	vec3 color = texture2D(colorBuffer, coord.xy).rgb;
	
	// Sample the glow image
	vec3 glowColor = texture2D(glow, coord.xy).rgb;
	
	// Blend the original color with our glow color
	color = color + glowColor;
	
	gl_FragColor = vec4(color, 1.0);
}