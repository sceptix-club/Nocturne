#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;  // For animated noise (can remove if not needed)

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

const vec2 size = vec2(800, 450);   // Framebuffer size
const float samples = 7.0;          // Pixels per axis; higher = bigger glow, worse performance
const float quality = 2.5;          // Defines size factor: Lower = smaller glow, better quality

// Sharpening kernel (simple approximation)
const float sharpenKernel[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

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
    float contrast = 1.0;  // Increase contrast
    finalColor.rgb = mix(vec3(0.5), finalColor.rgb, contrast);  // Apply contrast

    float saturation = 0.5;  // Slightly reduce saturation
    float luminance = dot(finalColor.rgb, vec3(0.299, 0.587, 0.114));  // Calculate luminance
    finalColor.rgb = mix(vec3(luminance), finalColor.rgb, saturation);  // Apply saturation

    // Sharpening filter
    vec2 texelSize = 1.0 / size;  // Get texel size to sample neighboring pixels
    vec4 sharpenedColor = vec4(0.0);

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            vec2 offset = vec2(i, j) * texelSize;
            vec4 sample = texture(texture0, fragTexCoord + offset);
            sharpenedColor += sample * sharpenKernel[(i + 1) * 3 + (j + 1)];
        }
    }

    // Blend original color with sharpened color using sharpness factor
    finalColor.rgb = mix(finalColor.rgb, sharpenedColor.rgb, 0.2);
}
