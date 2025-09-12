#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform float threshold;

out vec4 finalColor;

void main() {
    vec3 color = texture(texture0, fragTexCoord).rgb;

    float luminance = dot(color, vec3(0.299, 0.587, 0.114));
    if (luminance > threshold) {
        finalColor = vec4(color, 1.0);
    } else {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
