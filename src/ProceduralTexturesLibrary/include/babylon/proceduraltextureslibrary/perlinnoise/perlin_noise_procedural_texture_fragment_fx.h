﻿#ifndef BABYLON_PROCEDURAL_TEXTURES_LIBRARY_PERLINNOISE_PERLIN_NOISE_PROCEDURAL_TEXTURE_FRAGMENT_FX_H
#define BABYLON_PROCEDURAL_TEXTURES_LIBRARY_PERLINNOISE_PERLIN_NOISE_PROCEDURAL_TEXTURE_FRAGMENT_FX_H

namespace BABYLON {

extern const char* perlinNoiseProceduralTexturePixelShader;

const char* perlinNoiseProceduralTexturePixelShader
  = "// Taken and improved from https://www.shadertoy.com/view/MdGSzt\n"
    "\n"
    "#ifdef GL_ES\n"
    "precision highp float;\n"
    "#endif\n"
    "\n"
    "// Uniforms\n"
    "uniform float size;\n"
    "uniform float time;\n"
    "uniform float translationSpeed;\n"
    "\n"
    "// Varyings\n"
    "varying vec2 vUV;\n"
    "\n"
    "// Functions\n"
    "float r(float n)\n"
    "{\n"
    "   return fract(cos(n * 89.42) * 343.42);\n"
    "}\n"
    "\n"
    "vec2 r(vec2 n)\n"
    "{\n"
    "   return vec2(r(n.x * 23.62 - 300.0 + n.y * 34.35), r(n.x * 45.13 + 256.0 + n.y * 38.89)); \n"
    "}\n"
    "\n"
    "float worley(vec2 n,float s)\n"
    "{\n"
    "  float dis = 1.0;\n"
    "  for(int x = -1; x <= 1; x++)\n"
    "  {\n"
    "  for(int y = -1; y <= 1; y++)\n"
    "  {\n"
    "  vec2 p = floor(n / s) + vec2(x, y);\n"
    "  float d = length(r(p) + vec2(x, y) - fract(n / s));\n"
    "\n"
    "  if (dis > d)\n"
    "   dis = d;\n"
    "  }\n"
    "  }\n"
    "  return 1.0 - dis;\n"
    "}\n"
    "\n"
    "vec3 hash33(vec3 p3)\n"
    "{\n"
    "  p3 = fract(p3 * vec3(0.1031, 0.11369, 0.13787));\n"
    "  p3 += dot(p3, p3.yxz+19.19);\n"
    "  return -1.0 + 2.0 * fract(vec3((p3.x + p3.y) * p3.z, (p3.x + p3.z) * p3.y, (p3.y + p3.z) * p3.x));\n"
    "}\n"
    "\n"
    "float perlinNoise(vec3 p)\n"
    "{\n"
    "  vec3 pi = floor(p);\n"
    "  vec3 pf = p - pi;\n"
    "  \n"
    "  vec3 w = pf * pf * (3.0 - 2.0 * pf);\n"
    "  \n"
    "  return   mix(\n"
    "  mix(\n"
    "  mix(\n"
    "  dot(pf - vec3(0, 0, 0), hash33(pi + vec3(0, 0, 0))), \n"
    "  dot(pf - vec3(1, 0, 0), hash33(pi + vec3(1, 0, 0))),\n"
    "   w.x\n"
    "  ),\n"
    "  mix(\n"
    "  dot(pf - vec3(0, 0, 1), hash33(pi + vec3(0, 0, 1))), \n"
    "  dot(pf - vec3(1, 0, 1), hash33(pi + vec3(1, 0, 1))),\n"
    "   w.x\n"
    "  ),\n"
    "  w.z\n"
    "  ),\n"
    "  mix(\n"
    "  mix(\n"
    "  dot(pf - vec3(0, 1, 0), hash33(pi + vec3(0, 1, 0))), \n"
    "  dot(pf - vec3(1, 1, 0), hash33(pi + vec3(1, 1, 0))),\n"
    "   w.x\n"
    "  ),\n"
    "   mix(\n"
    "  dot(pf - vec3(0, 1, 1), hash33(pi + vec3(0, 1, 1))), \n"
    "  dot(pf - vec3(1, 1, 1), hash33(pi + vec3(1, 1, 1))),\n"
    "   w.x\n"
    "  ),\n"
    "  w.z\n"
    "  ),\n"
    "  w.y\n"
    "  );\n"
    "}\n"
    "\n"
    "// Main\n"
    "void main(void)\n"
    "{\n"
    "  vec2 uv = gl_FragCoord.xy + translationSpeed;\n"
    "\n"
    "  float dis = (\n"
    "  1.0 + perlinNoise(vec3(uv / vec2(size, size), time * 0.05) * 8.0))\n"
    "  * (1.0 + (worley(uv, 32.0)+ 0.5 * worley(2.0 * uv, 32.0) + 0.25 * worley(4.0 * uv, 32.0))\n"
    "  );\n"
    "\n"
    "  gl_FragColor = vec4(vec3(dis / 4.0), 1.0);\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_PROCEDURAL_TEXTURES_LIBRARY_PERLINNOISE_PERLIN_NOISE_PROCEDURAL_TEXTURE_FRAGMENT_FX_H