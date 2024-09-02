#version 330 core

in vec3 fragPosition;

out vec4 finalColor;

uniform samplerCube environmentMap;
uniform vec3 fogColor;
uniform float fogDensity;
uniform float fogStart;
uniform float fogEnd;

void main()
{
    // Calculate fog factor
    float distance = length(fragPosition);
    float fogFactor = clamp((fogEnd - distance) / (fogEnd - fogStart), 0.0, 1.0);

    vec4 color = mix(fogColor, vec4(1.0, 1.0, 1.0, 1.0), fogFactor);

    finalColor = color;
}