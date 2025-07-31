#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0; // Original scene
uniform sampler2D texture1; // Blurred bright pass
uniform float bloomIntensity; // Bloom strength (e.g., 0.5)

out vec4 finalColor;

void main() {
    vec3 original = texture(texture0, fragTexCoord).rgb;
    vec3 bloom = texture(texture1, fragTexCoord).rgb;

    // Additive blending with intensity control
    vec3 result = original + (bloom * bloomIntensity);

    // Optional: tone mapping to prevent over-brightness
    result = result / (1.0 + result); // Simple Reinhard tone mapping

    finalColor = vec4(result, 1.0);
}
