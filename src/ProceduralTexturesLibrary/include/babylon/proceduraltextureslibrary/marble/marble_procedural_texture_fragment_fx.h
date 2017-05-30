﻿#ifndef BABYLON_PROCEDURAL_TEXTURES_LIBRARY_MARBLE_MARBLE_PROCEDURAL_TEXTURE_FRAGMENT_FX_H
#define BABYLON_PROCEDURAL_TEXTURES_LIBRARY_MARBLE_MARBLE_PROCEDURAL_TEXTURE_FRAGMENT_FX_H

namespace BABYLON {

extern const char* marbleProceduralTexturePixelShader;

const char* marbleProceduralTexturePixelShader
  = "#ifdef GL_ES\n"
    "precision highp float;\n"
    "#endif\n"
    "\n"
    "varying vec2 vPosition;\n"
    "varying vec2 vUV;\n"
    "\n"
    "uniform float numberOfTilesHeight;\n"
    "uniform float numberOfTilesWidth;\n"
    "uniform float amplitude;\n"
    "uniform vec3 marbleColor;\n"
    "uniform vec3 jointColor;\n"
    "\n"
    "const vec3 tileSize = vec3(1.1, 1.0, 1.1);\n"
    "const vec3 tilePct = vec3(0.98, 1.0, 0.98);\n"
    "\n"
    "float rand(vec2 n) {\n"
    "  return fract(cos(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);\n"
    "}\n"
    "\n"
    "float noise(vec2 n) {\n"
    "  const vec2 d = vec2(0.0, 1.0);\n"
    "  vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));\n"
    "  return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);\n"
    "}\n"
    "\n"
    "float turbulence(vec2 P)\n"
    "{\n"
    "  float val = 0.0;\n"
    "  float freq = 1.0;\n"
    "  for (int i = 0; i < 4; i++)\n"
    "  {\n"
    "  val += abs(noise(P*freq) / freq);\n"
    "  freq *= 2.07;\n"
    "  }\n"
    "  return val;\n"
    "}\n"
    "\n"
    "float roundF(float number){\n"
    "  return sign(number)*floor(abs(number) + 0.5);\n"
    "}\n"
    "\n"
    "vec3 marble_color(float x)\n"
    "{\n"
    "  vec3 col;\n"
    "  x = 0.5*(x + 1.);\n"
    "  x = sqrt(x);             \n"
    "  x = sqrt(x);\n"
    "  x = sqrt(x);\n"
    "  col = vec3(.2 + .75*x);  \n"
    "  col.b *= 0.95;           \n"
    "  return col;\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "  float brickW = 1.0 / numberOfTilesWidth;\n"
    "  float brickH = 1.0 / numberOfTilesHeight;\n"
    "  float jointWPercentage = 0.01;\n"
    "  float jointHPercentage = 0.01;\n"
    "  vec3 color = marbleColor;\n"
    "  float yi = vUV.y / brickH;\n"
    "  float nyi = roundF(yi);\n"
    "  float xi = vUV.x / brickW;\n"
    "\n"
    "  if (mod(floor(yi), 2.0) == 0.0){\n"
    "  xi = xi - 0.5;\n"
    "  }\n"
    "\n"
    "  float nxi = roundF(xi);\n"
    "  vec2 brickvUV = vec2((xi - floor(xi)) / brickH, (yi - floor(yi)) / brickW);\n"
    "\n"
    "  if (yi < nyi + jointHPercentage && yi > nyi - jointHPercentage){\n"
    "  color = mix(jointColor, vec3(0.37, 0.25, 0.25), (yi - nyi) / jointHPercentage + 0.2);\n"
    "  }\n"
    "  else if (xi < nxi + jointWPercentage && xi > nxi - jointWPercentage){\n"
    "  color = mix(jointColor, vec3(0.44, 0.44, 0.44), (xi - nxi) / jointWPercentage + 0.2);\n"
    "  }\n"
    "  else {\n"
    "  float t = 6.28 * brickvUV.x / (tileSize.x + noise(vec2(vUV)*6.0));\n"
    "  t += amplitude * turbulence(brickvUV.xy);\n"
    "  t = sin(t);\n"
    "  color = marble_color(t);\n"
    "  }\n"
    "\n"
    "  gl_FragColor = vec4(color, 0.0);\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_PROCEDURAL_TEXTURES_LIBRARY_MARBLE_MARBLE_PROCEDURAL_TEXTURE_FRAGMENT_FX_H
