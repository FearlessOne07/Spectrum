#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0; // Original scene
uniform sampler2D texture1; // Blurred bright pass
uniform float bloomIntensity; // Bloom strength 

out vec4 finalColor;

void main() {
    vec3 original = texture(texture0, fragTexCoord).rgb;
    vec3 bloom = texture(texture1, fragTexCoord).rgb;


    vec3 result = original + (bloom * bloomIntensity);

    result = (result * (2.51 * result + 0.03)) / (result * (2.43 * result + 0.59) + 0.14);

    finalColor = vec4(result, 1.0);
}
