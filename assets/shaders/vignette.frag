#version 430 core

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float u_time;
uniform vec2 u_resolution;
uniform float u_vignetteStrength; // 0.0 = no effect, 1.0 = full

out vec4 finalColor;

void main() {
    vec2 uv = fragTexCoord;

    // Centered coordinates
    vec2 centered = uv - 0.5;

    // Apply aspect ratio correction
    centered.x *= u_resolution.x / u_resolution.y;

    // Distance from center
    float dist = length(centered);

    // Vignette strength ramp
    float vignetteFactor = smoothstep(0.5, 0.85, dist);

    // Final vignette mask (0.0 to 1.0)
    float vignetteAmount = vignetteFactor * u_vignetteStrength;

    // Original color
    vec4 texColor = texture(texture0, uv);

    // Apply vignette by darkening the color
    vec3 finalRgb = mix(texColor.rgb, vec3(1.0, 0.3, 0.3), vignetteAmount * abs(sin(u_time)));

    // Keep original alpha
    finalColor = vec4(finalRgb, texColor.a) * fragColor;
}
