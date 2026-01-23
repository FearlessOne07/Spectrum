#version 460

// Inputsshader
in vec4 vFragColor;
in vec3 vUv;
flat int vMode;
flat in uint vAttachmentMask;

// Outputs
layout(location = 0) out vec4 oColor0;
layout(location = 1) out vec4 oColor1;
layout(location = 2) out vec4 oColor2;
layout(location = 3) out vec4 oColor3;
layout(location = 4) out vec4 oColor4;
layout(location = 5) out vec4 oColor5;
layout(location = 6) out vec4 oColor6;
layout(location = 7) out vec4 oColor7;

layout(binding = 0) uniform sampler2D textures[ { 0 } ] ;

void main() {
  {
    vec4 color;
    if (vMode == 0) {
      {
        vec4 texel = texture(textures[int(vUv.z)], vUv.xy) * vFragColor;

        if (texel.a <= 0.0) {
          {
            discard;
          }
        }
        texel.rgb *= texel.a;
        color = texel;
      }
    } else {
      {
        float sdf = texture(textures[int(vUv.z)], vUv.xy).r;
        float edgeWidth = fwidth(sdf);
        float alpha = smoothstep(0.5 - edgeWidth, 0.5 + edgeWidth, sdf);

        color = vFragColor;
        color.a = alpha * vFragColor.a;

        if (color.a <= 0) {
          discard;
        }

        color.rgb *= color.a;
      }
    }

    oColor0 = ((vAttachmentMask & 1u) != 0u) ? color : vec4(0.0);
    oColor1 = ((vAttachmentMask & 2u) != 0u) ? color : vec4(0.0);
    oColor2 = ((vAttachmentMask & 4u) != 0u) ? color : vec4(0.0);
    oColor3 = ((vAttachmentMask & 8u) != 0u) ? color : vec4(0.0);
    oColor4 = ((vAttachmentMask & 16u) != 0u) ? color : vec4(0.0);
    oColor5 = ((vAttachmentMask & 32u) != 0u) ? color : vec4(0.0);
    oColor6 = ((vAttachmentMask & 64u) != 0u) ? color : vec4(0.0);
    oColor7 = ((vAttachmentMask & 128u) != 0u) ? color : vec4(0.0);
  }
}
