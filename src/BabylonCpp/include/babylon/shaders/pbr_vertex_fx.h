﻿#ifndef BABYLON_SHADERS_PBR_VERTEX_FX_H
#define BABYLON_SHADERS_PBR_VERTEX_FX_H

namespace BABYLON {

extern const char* pbrVertexShader;

const char* pbrVertexShader
  = "#ifdef GL_ES\n"
    "precision highp float;\n"
    "#endif\n"
    "\n"
    "// Attributes\n"
    "attribute vec3 position;\n"
    "#ifdef NORMAL\n"
    "attribute vec3 normal;\n"
    "#endif\n"
    "#ifdef UV1\n"
    "attribute vec2 uv;\n"
    "#endif\n"
    "#ifdef UV2\n"
    "attribute vec2 uv2;\n"
    "#endif\n"
    "#ifdef VERTEXCOLOR\n"
    "attribute vec4 color;\n"
    "#endif\n"
    "\n"
    "#include<bonesDeclaration>\n"
    "\n"
    "// Uniforms\n"
    "#include<instancesDeclaration>\n"
    "\n"
    "uniform mat4 view;\n"
    "uniform mat4 viewProjection;\n"
    "\n"
    "#ifdef ALBEDO\n"
    "varying vec2 vAlbedoUV;\n"
    "uniform mat4 albedoMatrix;\n"
    "uniform vec2 vAlbedoInfos;\n"
    "#endif\n"
    "\n"
    "#ifdef AMBIENT\n"
    "varying vec2 vAmbientUV;\n"
    "uniform mat4 ambientMatrix;\n"
    "uniform vec3 vAmbientInfos;\n"
    "#endif\n"
    "\n"
    "#ifdef OPACITY\n"
    "varying vec2 vOpacityUV;\n"
    "uniform mat4 opacityMatrix;\n"
    "uniform vec2 vOpacityInfos;\n"
    "#endif\n"
    "\n"
    "#ifdef EMISSIVE\n"
    "varying vec2 vEmissiveUV;\n"
    "uniform vec2 vEmissiveInfos;\n"
    "uniform mat4 emissiveMatrix;\n"
    "#endif\n"
    "\n"
    "#ifdef LIGHTMAP\n"
    "varying vec2 vLightmapUV;\n"
    "uniform vec2 vLightmapInfos;\n"
    "uniform mat4 lightmapMatrix;\n"
    "#endif\n"
    "\n"
    "#if defined(REFLECTIVITY) || defined(METALLICWORKFLOW) \n"
    "varying vec2 vReflectivityUV;\n"
    "uniform vec2 vReflectivityInfos;\n"
    "uniform mat4 reflectivityMatrix;\n"
    "#endif\n"
    "\n"
    "#ifdef BUMP\n"
    "varying vec2 vBumpUV;\n"
    "uniform vec3 vBumpInfos;\n"
    "uniform mat4 bumpMatrix;\n"
    "#endif\n"
    "\n"
    "#ifdef POINTSIZE\n"
    "uniform float pointSize;\n"
    "#endif\n"
    "\n"
    "// Output\n"
    "varying vec3 vPositionW;\n"
    "#ifdef NORMAL\n"
    "varying vec3 vNormalW;\n"
    "#endif\n"
    "\n"
    "#ifdef VERTEXCOLOR\n"
    "varying vec4 vColor;\n"
    "#endif\n"
    "\n"
    "\n"
    "#include<clipPlaneVertexDeclaration>\n"
    "#include<fogVertexDeclaration>\n"
    "#include<shadowsVertexDeclaration>[0..maxSimultaneousLights]\n"
    "\n"
    "#ifdef REFLECTIONMAP_SKYBOX\n"
    "varying vec3 vPositionUVW;\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_EQUIRECTANGULAR_FIXED\n"
    "varying vec3 vDirectionW;\n"
    "#endif\n"
    "\n"
    "#include<logDepthDeclaration>\n"
    "\n"
    "void main(void) {\n"
    "#ifdef REFLECTIONMAP_SKYBOX\n"
    "  vPositionUVW = position;\n"
    "#endif \n"
    "\n"
    "#include<instancesVertex>\n"
    "#include<bonesVertex>\n"
    "\n"
    "  gl_Position = viewProjection * finalWorld * vec4(position, 1.0);\n"
    "\n"
    "  vec4 worldPos = finalWorld * vec4(position, 1.0);\n"
    "  vPositionW = vec3(worldPos);\n"
    "\n"
    "#ifdef NORMAL\n"
    "  vNormalW = normalize(vec3(finalWorld * vec4(normal, 0.0)));\n"
    "#endif\n"
    "\n"
    "#ifdef REFLECTIONMAP_EQUIRECTANGULAR_FIXED\n"
    "  vDirectionW = normalize(vec3(finalWorld * vec4(position, 0.0)));\n"
    "#endif\n"
    "\n"
    "  // Texture coordinates\n"
    "#ifndef UV1\n"
    "  vec2 uv = vec2(0., 0.);\n"
    "#endif\n"
    "#ifndef UV2\n"
    "  vec2 uv2 = vec2(0., 0.);\n"
    "#endif\n"
    "\n"
    "#ifdef ALBEDO\n"
    "  if (vAlbedoInfos.x == 0.)\n"
    "  {\n"
    "  vAlbedoUV = vec2(albedoMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vAlbedoUV = vec2(albedoMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#ifdef AMBIENT\n"
    "  if (vAmbientInfos.x == 0.)\n"
    "  {\n"
    "  vAmbientUV = vec2(ambientMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vAmbientUV = vec2(ambientMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#ifdef OPACITY\n"
    "  if (vOpacityInfos.x == 0.)\n"
    "  {\n"
    "  vOpacityUV = vec2(opacityMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vOpacityUV = vec2(opacityMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#ifdef EMISSIVE\n"
    "  if (vEmissiveInfos.x == 0.)\n"
    "  {\n"
    "  vEmissiveUV = vec2(emissiveMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vEmissiveUV = vec2(emissiveMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#ifdef LIGHTMAP\n"
    "  if (vLightmapInfos.x == 0.)\n"
    "  {\n"
    "  vLightmapUV = vec2(lightmapMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vLightmapUV = vec2(lightmapMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#if defined(REFLECTIVITY) || defined(METALLICWORKFLOW) \n"
    "  if (vReflectivityInfos.x == 0.)\n"
    "  {\n"
    "  vReflectivityUV = vec2(reflectivityMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vReflectivityUV = vec2(reflectivityMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#ifdef BUMP\n"
    "  if (vBumpInfos.x == 0.)\n"
    "  {\n"
    "  vBumpUV = vec2(bumpMatrix * vec4(uv, 1.0, 0.0));\n"
    "  }\n"
    "  else\n"
    "  {\n"
    "  vBumpUV = vec2(bumpMatrix * vec4(uv2, 1.0, 0.0));\n"
    "  }\n"
    "#endif\n"
    "\n"
    "  // Clip plane\n"
    "#include<clipPlaneVertex>\n"
    "\n"
    "  // Fog\n"
    "#include<fogVertex>\n"
    "\n"
    "  // Shadows\n"
    "#include<shadowsVertex>[0..maxSimultaneousLights]\n"
    "\n"
    "  // Vertex color\n"
    "#ifdef VERTEXCOLOR\n"
    "  vColor = color;\n"
    "#endif\n"
    "\n"
    "  // Point size\n"
    "#ifdef POINTSIZE\n"
    "  gl_PointSize = pointSize;\n"
    "#endif\n"
    "\n"
    "  // Log. depth\n"
    "#include<logDepthVertex>\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_PBR_VERTEX_FX_H
