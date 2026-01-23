#version 460

in vec3 vUv;
layout(binding = 0) uniform sampler2D texture0; // Original scene
layout(binding = 1) uniform sampler2D blurredTexture; // Blurred bright pass
uniform float bloomIntensity; // Bloom strength

out vec4 finalColor;

void main() {
  vec3 original = texture(texture0, vUv.xy).rgb;
  vec3 bloom = texture(blurredTexture, vUv.xy).rgb;

  vec3 result = original + bloom * bloomIntensity;
  result = 1.0 - exp(-result);
  finalColor = vec4(result, 1.0);
}
