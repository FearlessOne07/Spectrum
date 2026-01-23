#version 330

in vec3 vUv;
uniform sampler2D texture0;
uniform vec2 resolution;
uniform vec2 direction;

out vec4 finalColor;

void main() {
    vec2 texelSize = 1.0 / resolution;
    float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    vec3 result = texture(texture0, vUv.xy).rgb * weights[0];
    float totalWeight = weights[0]; // Track actual weights used

    for(int i = 1; i < 5; ++i) {
        vec2 offset = direction * texelSize * float(i);

        // Check positive direction
        vec2 coord1 = vUv.xy + offset;
        if(coord1.x >= 0.0 && coord1.x <= 1.0 && coord1.y >= 0.0 && coord1.y <= 1.0) {
            result += texture(texture0, coord1).rgb * weights[i];
            totalWeight += weights[i];
        }

        // Check negative direction
        vec2 coord2 = vUv.xy - offset;
        if(coord2.x >= 0.0 && coord2.x <= 1.0 && coord2.y >= 0.0 && coord2.y <= 1.0) {
            result += texture(texture0, coord2).rgb * weights[i];
            totalWeight += weights[i];
        }
    }

    result = result / totalWeight;
    finalColor = vec4(result, 1.0);
}
