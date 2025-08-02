#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform float threshold; // Brightness threshold (e.g., 0.8)

out vec4 finalColor;

void main() {
    vec3 color = texture(texture0, fragTexCoord).rgb;

    // Calculate luminance using standard weights
    float luminance = dot(color, vec3(0.299, 0.587, 0.114));

    // Extract bright pixels above threshold
    if (luminance > threshold) {
        // Keep bright pixels, maybe boost them slightly
        finalColor = vec4(color, 1.0);
    } else {
        // Dark pixels become black
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
