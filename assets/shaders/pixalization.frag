#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add your custom variables here

// NOTE: Render size values must be passed from code
const float renderWidth = 1920;
const float renderHeight = 1080;

uniform float pixelWidth = 4.0;
uniform float pixelHeight = 4.0;

void main() {
    float dx = pixelWidth * (1.0 / renderWidth);
    float dy = pixelHeight * (1.0 / renderHeight);

    vec2 coord = vec2(dx * floor(fragTexCoord.x / dx), dy * floor(fragTexCoord.y / dy));

    vec4 tc = texture(texture0, coord);

    finalColor = tc;
}
