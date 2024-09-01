#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;  // For animated noise

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

const vec2 size = vec2(800, 450);   // Framebuffer size
const float samples = 7.0;          // Pixels per axis; higher = bigger glow, worse performance
const float quality = 2.5;          // Defines size factor: Lower = smaller glow, better quality

// Function to generate noise
float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1) / size * quality;

    // Texel color fetching from texture sampler
    vec4 source = texture(texture0, fragTexCoord);

    const int range = 2;  // Adjust range with samples

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            sum += texture(texture0, fragTexCoord + vec2(x, y) * sizeFactor);
        }
    }

    // Calculate glow effect
    vec4 glowColor = sum / (samples * samples);
    finalColor = (glowColor + source) * colDiffuse;

    // Vignette effect
    vec2 position = fragTexCoord * 2.0 - 1.0;  // Convert from [0,1] to [-1,1] range
    float vignette = smoothstep(0.5, 1.5, length(position));  // Adjusted for more dramatic vignette
    finalColor.rgb *= mix(vec3(1.0), vec3(0.0), vignette);    // Darken edges based on vignette

    // Cinematic color grading
    vec3 colorGrading = vec3(0.7, 0.8, 1.1);  // A slight cool tint (adjust to your preference)
    finalColor.rgb *= colorGrading;

    // Contrast and Saturation adjustment
    float contrast = 1.5;  // Increase contrast
    finalColor.rgb = mix(vec3(0.5), finalColor.rgb, contrast);  // Apply contrast

    float saturation = 0.5;  // Slightly reduce saturation
    float luminance = dot(finalColor.rgb, vec3(0.299, 0.587, 0.114));  // Calculate luminance
    finalColor.rgb = mix(vec3(luminance), finalColor.rgb, saturation);  // Apply saturation

    // Add camera noise
    float noiseStrength = 0.08;  // Adjust for stronger/weaker noise
    float noise = rand(fragTexCoord * size + vec2(time)) * noiseStrength;
    finalColor.rgb += noise;

    // RGB Chromatic Aberration
    float aberrationOffset = 0.002;  // Adjust for stronger/weaker aberration
    vec3 aberrationColor;
    aberrationColor.r = texture(texture0, fragTexCoord + vec2(aberrationOffset, 0.0)).r;
    aberrationColor.g = texture(texture0, fragTexCoord).g;
    aberrationColor.b = texture(texture0, fragTexCoord - vec2(aberrationOffset, 0.0)).b;
    
    finalColor.rgb = mix(finalColor.rgb, aberrationColor, 0.5);  // Blend with original color
}
