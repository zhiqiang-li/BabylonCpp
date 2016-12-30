﻿#ifndef BABYLON_SHADERS_SHADERS_INCLUDE_PBRLIGHTFUNCTIONS_FX_H
#define BABYLON_SHADERS_SHADERS_INCLUDE_PBRLIGHTFUNCTIONS_FX_H

namespace BABYLON {

extern const char* pbrLightFunctions;

const char* pbrLightFunctions
  = "// Light Computing\n"
    "struct lightingInfo\n"
    "{\n"
    "  vec3 diffuse;\n"
    "  #ifdef SPECULARTERM\n"
    "  vec3 specular;\n"
    "  #endif\n"
    "};\n"
    "\n"
    "float computeDistanceLightFalloff(vec3 lightOffset, float lightDistanceSquared, float range)\n"
    "{   \n"
    "  #ifdef USEPHYSICALLIGHTFALLOFF\n"
    "  float lightDistanceFalloff = 1.0 / ((lightDistanceSquared + 0.0001));\n"
    "  #else\n"
    "  float lightDistanceFalloff = max(0., 1.0 - length(lightOffset) / range);\n"
    "  #endif\n"
    "  \n"
    "  return lightDistanceFalloff;\n"
    "}\n"
    "\n"
    "float computeDirectionalLightFalloff(vec3 lightDirection, vec3 directionToLightCenterW, float lightAngle, float exponent)\n"
    "{\n"
    "  float falloff = 0.0;\n"
    "  \n"
    "  #ifdef USEPHYSICALLIGHTFALLOFF\n"
    "  float cosHalfAngle = cos(lightAngle * 0.5);\n"
    "  const float kMinusLog2ConeAngleIntensityRatio = 6.64385618977; // -log2(0.01)\n"
    "\n"
    "  // Calculate a Spherical Gaussian (von Mises-Fisher distribution, not angle-based Gaussian) such that the peak is in the light direction,\n"
    "  // and the value at the nominal cone angle is 1% of the peak. Because we want the distribution to decay from unity (100%)\n"
    "  // at the peak direction (dot product = 1) down to 1% at the nominal cone cutoff (dot product = cosAngle) \n"
    "  // the falloff rate expressed in terms of the base-two dot product is therefore -log2(ConeAngleIntensityRatio) / (1.0 - cosAngle).\n"
    "  // Note that the distribution is unnormalised in that peak density is unity, rather than the total energy is unity.\n"
    "  float concentrationKappa = kMinusLog2ConeAngleIntensityRatio / (1.0 - cosHalfAngle);\n"
    "  \n"
    "  // Evaluate spherical gaussian for light directional falloff for spot light type (note: spot directional falloff; \n"
    "  // not directional light type)\n"
    "  vec4 lightDirectionSpreadSG = vec4(-lightDirection * concentrationKappa, -concentrationKappa);\n"
    "  falloff = exp2(dot(vec4(directionToLightCenterW, 1.0), lightDirectionSpreadSG));\n"
    "  #else\n"
    "  float cosAngle = max(0.000000000000001, dot(-lightDirection, directionToLightCenterW));\n"
    "  if (cosAngle >= lightAngle)\n"
    "  {\n"
    "  falloff = max(0., pow(cosAngle, exponent));\n"
    "  }\n"
    "  #endif\n"
    "  \n"
    "  return falloff;\n"
    "}\n"
    "\n"
    "lightingInfo computeLighting(vec3 viewDirectionW, vec3 vNormal, vec4 lightData, vec3 diffuseColor, vec3 specularColor, float rangeRadius, float roughness, float NdotV, vec3 reflectance90, out float NdotL) {\n"
    "  lightingInfo result;\n"
    "\n"
    "  vec3 lightDirection;\n"
    "  float attenuation = 1.0;\n"
    "  float lightDistance;\n"
    "  \n"
    "  // Point\n"
    "  if (lightData.w == 0.)\n"
    "  {\n"
    "  vec3 lightOffset = lightData.xyz - vPositionW;\n"
    "  float lightDistanceSquared = dot(lightOffset, lightOffset);\n"
    "  attenuation = computeDistanceLightFalloff(lightOffset, lightDistanceSquared, rangeRadius);\n"
    "  \n"
    "  lightDistance = sqrt(lightDistanceSquared);\n"
    "  lightDirection = normalize(lightOffset);\n"
    "  }\n"
    "  // Directional\n"
    "  else\n"
    "  {\n"
    "  lightDistance = length(-lightData.xyz);\n"
    "  lightDirection = normalize(-lightData.xyz);\n"
    "  }\n"
    "  \n"
    "  // Roughness\n"
    "  roughness = adjustRoughnessFromLightProperties(roughness, rangeRadius, lightDistance);\n"
    "  \n"
    "  // diffuse\n"
    "  vec3 H = normalize(viewDirectionW + lightDirection);\n"
    "  NdotL = max(0.00000000001, dot(vNormal, lightDirection));\n"
    "  float VdotH = clamp(0.00000000001, 1.0, dot(viewDirectionW, H));\n"
    "\n"
    "  float diffuseTerm = computeDiffuseTerm(NdotL, NdotV, VdotH, roughness);\n"
    "  result.diffuse = diffuseTerm * diffuseColor * attenuation;\n"
    "\n"
    "  #ifdef SPECULARTERM\n"
    "  // Specular\n"
    "  float NdotH = max(0.00000000001, dot(vNormal, H));\n"
    "\n"
    "  vec3 specTerm = computeSpecularTerm(NdotH, NdotL, NdotV, VdotH, roughness, specularColor, reflectance90);\n"
    "  result.specular = specTerm * attenuation;\n"
    "  #endif\n"
    "\n"
    "  return result;\n"
    "}\n"
    "\n"
    "lightingInfo computeSpotLighting(vec3 viewDirectionW, vec3 vNormal, vec4 lightData, vec4 lightDirection, vec3 diffuseColor, vec3 specularColor, float rangeRadius, float roughness, float NdotV, vec3 reflectance90, out float NdotL) {\n"
    "  lightingInfo result;\n"
    "\n"
    "  vec3 lightOffset = lightData.xyz - vPositionW;\n"
    "  vec3 directionToLightCenterW = normalize(lightOffset);\n"
    "\n"
    "  // Distance falloff.\n"
    "  float lightDistanceSquared = dot(lightOffset, lightOffset);\n"
    "  float attenuation = computeDistanceLightFalloff(lightOffset, lightDistanceSquared, rangeRadius);\n"
    "  \n"
    "  // Directional falloff.\n"
    "  float directionalAttenuation = computeDirectionalLightFalloff(lightDirection.xyz, directionToLightCenterW, lightDirection.w, lightData.w);\n"
    "  attenuation *= directionalAttenuation;\n"
    "  \n"
    "  // Roughness.\n"
    "  float lightDistance = sqrt(lightDistanceSquared);\n"
    "  roughness = adjustRoughnessFromLightProperties(roughness, rangeRadius, lightDistance);\n"
    "  \n"
    "  // Diffuse\n"
    "  vec3 H = normalize(viewDirectionW - lightDirection.xyz);\n"
    "  NdotL = max(0.00000000001, dot(vNormal, -lightDirection.xyz));\n"
    "  float VdotH = clamp(dot(viewDirectionW, H), 0.00000000001, 1.0);\n"
    "\n"
    "  float diffuseTerm = computeDiffuseTerm(NdotL, NdotV, VdotH, roughness);\n"
    "  result.diffuse = diffuseTerm * diffuseColor * attenuation;\n"
    "\n"
    "  #ifdef SPECULARTERM\n"
    "  // Specular\n"
    "  float NdotH = max(0.00000000001, dot(vNormal, H));\n"
    "\n"
    "  vec3 specTerm = computeSpecularTerm(NdotH, NdotL, NdotV, VdotH, roughness, specularColor, reflectance90);\n"
    "  result.specular = specTerm  * attenuation;\n"
    "  #endif\n"
    "\n"
    "  return result;\n"
    "}\n"
    "\n"
    "lightingInfo computeHemisphericLighting(vec3 viewDirectionW, vec3 vNormal, vec4 lightData, vec3 diffuseColor, vec3 specularColor, vec3 groundColor, float roughness, float NdotV, vec3 reflectance90, out float NdotL) {\n"
    "  lightingInfo result;\n"
    "\n"
    "  // Roughness\n"
    "  // Do not touch roughness on hemispheric.\n"
    "\n"
    "  // Diffuse\n"
    "  NdotL = dot(vNormal, lightData.xyz) * 0.5 + 0.5;\n"
    "  result.diffuse = mix(groundColor, diffuseColor, NdotL);\n"
    "\n"
    "  #ifdef SPECULARTERM\n"
    "  // Specular\n"
    "  vec3 lightVectorW = normalize(lightData.xyz);\n"
    "  vec3 H = normalize(viewDirectionW + lightVectorW);\n"
    "  float NdotH = max(0.00000000001, dot(vNormal, H));\n"
    "  NdotL = max(0.00000000001, NdotL);\n"
    "  float VdotH = clamp(0.00000000001, 1.0, dot(viewDirectionW, H));\n"
    "\n"
    "  vec3 specTerm = computeSpecularTerm(NdotH, NdotL, NdotV, VdotH, roughness, specularColor, reflectance90);\n"
    "  result.specular = specTerm;\n"
    "  #endif\n"
    "\n"
    "  return result;\n"
    "}\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_SHADERS_INCLUDE_PBRLIGHTFUNCTIONS_FX_H
