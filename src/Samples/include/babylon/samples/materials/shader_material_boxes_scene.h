#ifndef BABYLON_SAMPLES_MATERIALS_SHADER_MATERIAL_BOXES_SCENE_H
#define BABYLON_SAMPLES_MATERIALS_SHADER_MATERIAL_BOXES_SCENE_H

#include <babylon/interfaces/irenderable_scene.h>
#include <babylon/materials/effect_shaders_store.h>

namespace BABYLON {
namespace Samples {

class ShaderMaterialBoxesScene : public IRenderableScene {

public:
  static constexpr const char* customVertexShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "// Attributes\n"
      "attribute vec3 position;\n"
      "attribute vec3 normal;\n"
      "attribute vec2 uv;\n"
      "\n"
      "// Uniforms\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float time;\n"
      "\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "void main(void) {\n"
      "  vec3 v = position;\n"
      "  gl_Position = worldViewProjection * vec4(v, 1.0);\n"
      "  vPosition = position;\n"
      "  vNormal = normal;\n"
      "  vUV = uv;\n"
      "}\n";

  static constexpr const char* custom1FragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float time;\n"
      "\n"
      "float rand(vec2 n) {\n"
      "  return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);\n"
      "}\n"
      "\n"
      "float noise(vec2 n) {\n"
      "  const vec2 d = vec2(0.0, 1.0);\n"
      "  vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));\n"
      "  return mix(mix(rand(b), rand(b + d.yx), f.x),\n"
      "             mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);\n"
      "}\n"
      "\n"
      "float fbm(vec2 n) {\n"
      "  float total = 0.0, amplitude = 1.0;\n"
      "  for (int i = 0; i < 7; i++) {\n"
      "    total += noise(n) * amplitude;\n"
      "    n += n;\n"
      "    amplitude *= 0.5;\n"
      "  }\n"
      "  return total;\n"
      "}\n"
      "\n"
      "void main(void) {\n"
      "  vec2 t = vUV * vec2(2.0, 1.0) - time * 3.0;\n"
      "  vec2 t2 = (vec2(1, -1) + vUV) * vec2(2.0, 1.0) - time * 3.0;\n"
      "  float ycenter = fbm(t) * 0.5;\n"
      "  float ycenter2 = fbm(t2) * 0.5;\n"
      "  float diff = abs(vUV.y - ycenter);\n"
      "  float c1 = 1.0 - mix(0.0, 1.0, diff * 20.0);\n"
      "  float diff2 = abs(vUV.y - ycenter2);\n"
      "  float c2 = 1.0 - mix(0.0, 1.0, diff2 * 20.0);\n"
      "  float c = max(c1, c2);\n"
      "  vec3 cout = vec3(c * 0.2, 0.6 * c2, c);\n"
      "  if (cout.x < .2 && cout.y < .2 && cout.z < .2) {\n"
      "    if (mod(vUV.x * 50., 2.0) < 1.7 && mod(vUV.y * 50., 2.0) < 1.7) {\n"
      "      discard;\n"
      "    } else {\n"
      "      cout = vec3(0.7, 0.7, 0.7);\n"
      "    }\n"
      "  }\n"
      "\n"
      "  gl_FragColor = vec4(cout, 1.);\n"
      "}\n";

  static constexpr const char* custom2FragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float time;\n"
      "\n"
      "void main(void) {\n"
      "  vec2 mv = vec2(.5, .5);\n"
      "  float r = .3 + mod(time * .1, .4);\n"
      "  vec3 color = vec3(smoothstep(r, r + .01, distance(mv, vUV)));\n"
      "  if (color.x < .1 && color.y < .1)\n"
      "    discard;\n"
      "  gl_FragColor = vec4(color, 1.);\n"
      "}\n";

  static constexpr const char* custom3FragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float time;\n"
      "\n"
      "float PI = 3.141592;\n"
      "float STROKE = 0.3;\n"
      "\n"
      "void main(void) {\n"
      "  vec2 uv = vUV.xy * 5.;\n"
      "  float freq1 = 0.5 * sin(time * 4. + uv.x * .125 + uv.y * .2) + 0.5;\n"
      "  float circle = smoothstep(freq1 - STROKE, freq1,\n"
      "                            cos(uv.x * 2.0 * PI) * cos(uv.y * 2.0 *\n"
      "                            PI)) -\n"
      "                 smoothstep(freq1, freq1 + STROKE,\n"
      "                            cos(uv.x * 2.0 * PI) * cos(uv.y * 2.0 *\n"
      "                            PI));\n"
      "  gl_FragColor = vec4(circle * 0.2, circle * 0.3, circle, 1.);\n"
      "}\n";

  // Latt1c3 ( https://www.shadertoy.com/view/MlX3RB )
  static constexpr const char* custom4FragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float time;\n"
      "\n"
      "mat3 xrot(float t) {\n"
      "  return mat3(1.0, 0.0, 0.0,\n"
      "              0.0, cos(t), -sin(t),\n"
      "              0.0, sin(t), cos(t));\n"
      "}\n"
      "\n"
      "mat3 yrot(float t) {\n"
      "  return mat3(cos(t), 0.0, -sin(t),\n"
      "              0.0, 1.0, 0.0,\n"
      "              sin(t), 0.0, cos(t));\n"
      "}\n"
      "\n"
      "mat3 zrot(float t) {\n"
      "  return mat3(cos(t), -sin(t), 0.0,\n"
      "              sin(t), cos(t), 0.0,\n"
      "              0.0, 0.0,1.0);\n"
      "}\n"
      "\n"
      "void main(void) {\n"
      "  float PI = 3.141592;\n"
      "  float STROKE = 0.3;\n"
      "  vec2 uv = vUV.xy * 1.;\n"
      "  vec3 eye = normalize(vec3(uv, 1.0 - dot(uv, uv) * 0.5));\n"
      "  float tt = 0.0;\n"
      "  float d = 0.0;\n"
      "  vec3 col;\n"
      "  for (int i = 0; i < 16; ++i) {\n"
      "    vec3 pos = eye * tt;\n"
      "    pos = pos * xrot(-PI / 4.0) * yrot(-PI / 4.0);\n"
      "    float theta = time;\n"
      "    pos = pos * xrot(theta) * yrot(theta) * zrot(theta);\n"
      "    pos.z += time;\n"
      "    pos.y += 0.25 + time;\n"
      "    pos.x += 0.5 + time;\n"
      "    vec3 coord = floor(pos);\n"
      "    pos = (pos - coord) - 0.5;\n"
      "    d = length(pos) - 0.2;\n"
      "    float idx = dot(coord, vec3(1.0));\n"
      "    idx = floor(fract(idx / 3.0) * 3.0);\n"
      "    if (idx == 0.0) {\n"
      "      col = vec3(1.0, 0.0, 0.0);\n"
      "    } else if (idx == 1.0) {\n"
      "      col = vec3(0.0, 1.0, 0.0);\n"
      "    } else if (idx == 2.0) {\n"
      "      col = vec3(0.0, 0.0, 1.0);\n"
      "    }\n"
      "    float k;\n"
      "    k = length(pos.xy) - 0.05;\n"
      "    if (k < d) {\n"
      "      d = k;\n"
      "      col = vec3(1.0, 1.0, 1.0);\n"
      "    }\n"
      "    k = length(pos.xz) - 0.05;\n"
      "    if (k < d) {\n"
      "      d = k;\n"
      "      col = vec3(1.0, 1.0, 1.0);\n"
      "    }\n"
      "    k = length(pos.yz) - 0.05;\n"
      "    if (k < d) {\n"
      "      d = k;\n"
      "      col = vec3(1.0, 1.0, 1.0);\n"
      "    }\n"
      "    tt += d;\n"
      "  }\n"
      "  float fog = 1.0 / (1.0 + tt * tt * 0.5 + d * 100.0);\n"
      "  gl_FragColor = vec4(fog * col, 1.);\n"
      "}\n";

  static constexpr const char* custom5FragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float time;\n"
      "\n"
      "float r(float n) { return fract(abs(sin(n * 55.753) * 367.34)); }\n"
      "float r(vec2 n) { return r(dot(n, vec2(2.46, -1.21))); }\n"
      "\n"
      "vec3 color(float type) {\n"
      "  float t = floor(type * 3.0);\n"
      "  vec3 c1 = vec3(0.738, 0.067, 0.185);\n"
      "  vec3 c2 = vec3(1.0);\n"
      "  vec3 c3 = vec3(0.15);\n"
      "  return mix(c1, mix(c2, c3, t - 1.0), clamp(t, 0.0, 1.0));\n"
      "}\n"
      "\n"
      "void main(void) {\n"
      "  float PI = 3.141592;\n"
      "  float STROKE = 0.3;\n"
      "  vec2 uv = vUV.xy * 300.;\n"
      "  float a = (radians(60.0));\n"
      "  float zoom = 192.0;\n"
      "  vec2 cc = (uv.xy + vec2(time * zoom, 0.0)) * vec2(sin(a), 1.0);\n"
      "  cc = ((cc + vec2(cc.y, 0.0) * cos(a)) / zoom) +\n"
      "       vec2(floor((cc.x - cc.y * cos(a)) / zoom * 4.0) / 4.0, 0.0);\n"
      "  float type = (r(floor(cc * 4.0)) * 0.2 + r(floor(cc * 2.0)) * 0.3 +\n"
      "                r(floor(cc)) * 0.5);\n"
      "  vec3 n = color(type);\n"
      "  float l = fract((fract(cc.y * 4.0) + fract(cc.x * 4.0) +\n"
      "                   fract((cc.x - cc.y) * 4.0) * 0.5) /\n"
      "                  2.5) *\n"
      "                0.3 +\n"
      "            0.7;\n"
      "  gl_FragColor = vec4(n * l, 1.);\n"
      "}\n";

  static constexpr const char* custom6FragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float time;\n"
      "\n"
      "void main(void) {\n"
      "  float PI = 3.141592;\n"
      "  vec2 uv = vUV.xy * 300.;\n"
      "  float STROKE = 0.3;\n"
      "  float mag = 10.;\n"
      "  float pulse = 0.;\n"
      "  float gs = 500. / 20.;\n"
      "  float ar = 1.;\n"
      "  vec2 s = uv.xy / mag + pulse;\n"
      "  vec2 p = uv.xy / 500. * s - s / 2.;\n"
      "  float v = .0 + sin((p.x + .8 * time)) + sin((p.y + .8 * time) / 2.) \n"
      "            + sin((p.x + p.y + .9 * time) / 2.);\n"
      "  p += s / 2. * vec2(sin(time / .9), cos(time / .6));\n"
      "  v += sin(sqrt(p.x * p.x + p.y * p.y + 1.) + time);\n"
      "  float R = sin(.2 * PI * v);\n"
      "  float G = cos(.75 * PI * v);\n"
      "  float B = sin(.9 * PI * v);\n"
      "  R = ceil(R * 255. / 8.) * 8. / 256.;\n"
      "  G = ceil(G * 255. / 16.) * 16. / 256.;\n"
      "  B = ceil(B * 255. / 8.) * 8. / 256.;\n"
      "  if (mod(R, 16.) < 1.)\n"
      "    R = G * .5 + .5;\n"
      "  vec3 col = vec3(R, G, B);\n"
      "  col *= 0.4 * 1. / length(sin(1. * .1 * gs * p.x));\n"
      "  col *= 0.8 * 1. / length(sin(ar * .1 * gs * p.y));\n"
      "  col *= .33 * length(sin(5. * p.y * gs));\n"
      "  col = clamp(col, vec3(.0), vec3(1.));\n"
      "  gl_FragColor = vec4(col, 1.);\n"
      "}\n";

  static constexpr const char* custom7FragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float time;\n"
      "\n"
      "float dist(vec2 uv, vec2 p0) {\n"
      "  vec2 off = abs(uv - p0) * 3.0;\n"
      "  float p = sin(time) * 2.0 + 4.0;\n"
      "  float r = pow(pow(off.x, p) + pow(off.y, p), 1.0 / p);\n"
      "  vec2 ox = uv - p;\n"
      "  float t = atan(ox.y, ox.x);\n"
      "  return 1.0 -\n"
      "         abs(1.0 - pow(mod(r * 8.0 / \n"
      "                (3.5 + sin(t * 4.0 + time)), 1.0), 2.0));\n"
      "}\n"
      "\n"
      "void main(void) {\n"
      "  float PI = 3.141592;\n"
      "  vec2 uv = vUV;\n"
      "  float d1 = dist(uv, vec2(0.3, 0.3));\n"
      "  float d2 = dist(uv, vec2(0.7, 0.3));\n"
      "  float d3 = dist(uv, vec2(0.3, 0.7));\n"
      "  float d4 = dist(uv, vec2(0.7, 0.7));\n"
      "  float d = d1 - d2 - d3 + d4;\n"
      "  gl_FragColor = vec4(d, d, d, 1.);\n"
      "}\n";

  // Tileable Water Caustic ( https://www.shadertoy.com/view/MdlXz8 )
  static constexpr const char* custom8FragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float time;\n"
      "\n"
      "float rand(vec2 n) {\n"
      "  return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);\n"
      "}\n"
      "\n"
      "float noise(vec2 n) {\n"
      "  const vec2 d = vec2(0.0, 1.0);\n"
      "  vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));\n"
      "  return mix(mix(rand(b), rand(b + d.yx), f.x),\n"
      "             mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);\n"
      "}\n"
      "\n"
      "float fbm(vec2 n) {\n"
      "  float total = 0.0, amplitude = 1.0;\n"
      "  for (int i = 0; i < 7; i++) {\n"
      "    total += noise(n) * amplitude;\n"
      "    n += n;\n"
      "    amplitude *= 0.5;\n"
      "  }\n"
      "  return total;\n"
      "}\n"
      "\n"
      "mat2 rotate2d(float angle) {\n"
      "  return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));\n"
      "}\n"
      "\n"
      "float variation(vec2 v1, vec2 v2, float strength, float speed) {\n"
      "  return sin(dot(normalize(v1), normalize(v2)) * strength + time *\n"
      "         speed) / 100.0;\n"
      "}\n"
      "\n"
      "vec3 paintCircle(vec2 uv, vec2 center, float rad, float width) {\n"
      "\n"
      "  vec2 diff = center - uv;\n"
      "  float len = length(diff);\n"
      "\n"
      "  len += variation(diff, vec2(0.0, 1.0), 5.0, 2.0);\n"
      "  len -= variation(diff, vec2(1.0, 0.0), 5.0, 2.0);\n"
      "\n"
      "  float circle =\n"
      "      smoothstep(rad - width, rad, len) - \n"
      "      smoothstep(rad, rad + width, len);\n"
      "  return vec3(circle);\n"
      "}\n"
      "void main(void) {\n"
      "  float PI = 3.141592;\n"
      "  vec2 uv = vUV.xy * 1.01;\n"
      "  \n"
      "  float TAU = 6.28318530718;\n"
      "  vec2 p = mod(uv * TAU, TAU) - 250.0;\n"
      "  vec2 i = vec2(p);\n"
      "  float c = 1.0;\n"
      "  float inten = .005;\n"
      "  for (int n = 0; n < 5; n++) {\n"
      "    float t = time * (1.0 - (3.5 / float(n + 1)));\n"
      "    i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y)\n"
      "        + cos(t + i.x));\n"
      "    c += 1.0 / length(vec2(p.x / (sin(i.x + t) / inten),\n"
      "                           p.y / (cos(i.y + t) / inten)));\n"
      "  }\n"
      "  c /= float(5);\n"
      "  c = 1.17 - pow(c, 1.4);\n"
      "  vec3 colour = vec3(pow(abs(c), 8.0));\n"
      "  colour = clamp(colour + vec3(0.0, 0.35, 0.5), 0.0, 1.0);\n"
      "  // Flash tile borders...\n"
      "  vec2 pixel = 2.0 / vec2(50., 50.).xy;\n"
      "  uv *= 2.0;\n"
      "\n"
      "  // Flash value.\n"
      "  float f = 0.; // floor(mod(time*.5, 2.0));\n"
      "  // Rule out first screen pixels and flash.\n"
      "  vec2 first = step(pixel, uv) * f;\n"
      "  // Add one line of pixels per tile.\n"
      "  uv = step(fract(uv), pixel);\n"
      "  // Yellow line\n"
      "  colour = mix(colour, vec3(1.0, 1.0, 0.0),\n"
      "               (uv.x + uv.y) * first.x * first.y);\n"
      "  gl_FragColor = vec4(colour, 1.);\n"
      "}\n";

  // Shiny Circle ( https://www.shadertoy.com/view/ltBXRc )
  static constexpr const char* custom9FragmentShader
    = "#ifdef GL_ES\n"
      "precision highp float;\n"
      "#endif\n"
      "\n"
      "varying vec3 vPosition;\n"
      "varying vec3 vNormal;\n"
      "varying vec2 vUV;\n"
      "\n"
      "uniform mat4 worldViewProjection;\n"
      "uniform float time;\n"
      "\n"
      "mat2 rotate2d(float angle) {\n"
      "  return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));\n"
      "}\n"
      "\n"
      "float variation(vec2 v1, vec2 v2, float strength, float speed) {\n"
      "  return sin(dot(normalize(v1), normalize(v2))\n"
      "                 * strength +\n"
      "             time * speed) /\n"
      "         100.0;\n"
      "}\n"
      "\n"
      "vec3 paintCircle(vec2 uv, vec2 center, float rad, float width) {\n"
      "  vec2 diff = center - uv;\n"
      "  float len = length(diff);\n"
      "  len += variation(diff, vec2(0.0, 1.0), 5.0, 2.0);\n"
      "  len -= variation(diff, vec2(1.0, 0.0), 5.0, 2.0);\n"
      "  float circle = smoothstep(rad - width, rad, len)\n"
      "                 - smoothstep(rad, rad + width, len);\n"
      "  return vec3(circle);\n"
      "}\n"
      "\n"
      "void main(void) {\n"
      "  float PI = 3.141592;\n"
      "  vec2 uv = vUV.xy * 1.01;\n"
      "  vec3 color;\n"
      "  float radius = 0.35;\n"
      "  vec2 center = vec2(0.5);\n"
      "  color = paintCircle(uv, center, radius, 0.1);\n"
      "  vec2 v = rotate2d(time) * uv;\n"
      "  color *= vec3(v.x, v.y, 0.7 - v.y * v.x);\n"
      "  color += paintCircle(uv, center, radius, 0.01);\n"
      "  gl_FragColor = vec4(color, 1.);\n"
      "}\n";

public:
  ShaderMaterialBoxesScene(ICanvas* iCanvas);
  ~ShaderMaterialBoxesScene();

  const char* getName() override;
  void initializeScene(ICanvas* canvas, Scene* scene) override;

private:
  EffectShadersStore _effectShadersStore;
  float _time;
  std::array<ShaderMaterial*, 9> _shaderMaterials;

}; // end of class ShaderMaterialBoxesScene

} // end of namespace Samples
} // end of namespace BABYLON

#endif // end of BABYLON_SAMPLES_MATERIALS_SHADER_MATERIAL_BOXES_SCENE_H
