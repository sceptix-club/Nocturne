#version 330

in vec2 fragTexCoord;

out vec4 finalColor;

uniform sampler2D texture0;

uniform float radius;

uniform float blur;

uniform vec3 color;

void main() {
    finalColor = mix(texture(texture0, fragTexCoord), vec4(color, 1.0), smoothstep(radius, radius + blur, distance(fragTexCoord, vec2(0.5, 0.5)))); // Maybe add some sort of feature to change vec2(X, Y) - vignettePositionX/Y
}