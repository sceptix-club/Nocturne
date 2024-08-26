#version 330 core

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Input uniform values
uniform samplerCube environmentMap;
uniform bool vflipped;
uniform bool doGamma;

out vec4 FragColor; // Use out instead of gl_FragColor for GLSL 3.3

void main()
{
    // Fetch color from texture map
    vec4 texelColor = vec4(0.0);

    if (vflipped) 
        texelColor = texture(environmentMap, vec3(-fragPosition.x, fragPosition.y, -fragPosition.z));
    else 
        texelColor = texture(environmentMap, fragPosition);

    vec3 color = vec3(texelColor.x, texelColor.y, texelColor.z);

    if (doGamma) // Apply gamma correction
    {
        color = color / (color + vec3(1.0));
        color = pow(color, vec3(1.0 / 2.2));
    }

    // Calculate final fragment color
    FragColor = vec4(color, 1.0);
}
