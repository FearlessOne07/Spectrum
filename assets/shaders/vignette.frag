#version 430 core

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float u_time;
uniform vec2 u_resolution;
uniform float u_vignetteStrength;

out vec4 finalColor;

void main() {
    vec2 uv = fragTexCoord;

    // Center the coordinates
    vec2 centered = uv - 0.5;

    // Apply aspect ratio correction
    centered.x *= u_resolution.x / u_resolution.y;

    // Calculate distance from center
    float dist = length(centered);

    // Create vignette effect
    // Inner radius: 0.4, Outer radius: 0.8 (adjust these for size)
    float vignette = smoothstep(0.1, 1.0, dist);

    // Sample the original texture
    vec4 texColor = texture(texture0, uv);

    // Mix original color with red tint based on vignette
    float finalAlpha = max(texColor.a, vignette * 1);
    vec3 redTint = mix(texColor.rgb, vec3(1.0, 0.3, 0.3), vignette * u_vignetteStrength * abs(sin(u_time * 1.5))); // Added multiplier for subtlety

    // Output final color
    finalColor = vec4(redTint, finalAlpha) * fragColor;
}
