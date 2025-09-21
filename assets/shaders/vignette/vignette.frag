#version 330 core
in vec2 fragTexCoord;

uniform sampler2D texture0;

// custom
const float uVignetteThickness = 0.07;

uniform vec2 uResolution; // pass screen width/height
uniform vec4 uVignetteColor;
uniform float uVignetteStrength;

out vec4 oFragColor;

float roundedBoxSDF(vec2 p, vec2 halfSize, float radius) {
    vec2 d = abs(p) - halfSize + radius;
    return (length(max(d, 0.0)) + min(max(d.x, d.y), 0.0)) - radius;
}

void main()
{
    vec4 color = texture(texture0, fragTexCoord);

    // shift to center and correct aspect ratio
    vec2 uv = fragTexCoord.xy - 0.5;

    vec2 halfSize = vec2(0.5 - uVignetteThickness);

    float distToEdge = roundedBoxSDF(uv, halfSize, 0.02);
    float vignette = smoothstep(0.0, uVignetteThickness, distToEdge);
    oFragColor = mix(color, vec4(uVignetteColor.rgb, 1), vignette * uVignetteStrength);
}
