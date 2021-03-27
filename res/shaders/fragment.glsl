#version 330 core

#ifndef POINT_LIGHT_COUNT
#define POINT_LIGHT_COUNT 16
#endif

struct PointLight{
  vec3 vColor; // Color of that point light
  vec3 vPosition;

  float fAmbient;

  float fConstantAtt;
  float fLinearAtt;
  float fExpAtt;
};

in vec4 texcoord;
flat in float frag_textureId;
in vec3 normal;
in vec3 positionFrag;

uniform sampler2DArray textureArray;
uniform PointLight pointLights[POINT_LIGHT_COUNT];

vec3 getPointLightColor(const PointLight ptLight, vec3 vWorldPos, vec3 vNormal)
{
  vec3 vPosToLight = vWorldPos-ptLight.vPosition;
  float fDist = length(vPosToLight);
  vPosToLight = normalize(vPosToLight);

  float fDiffuse = max(0.0, dot(vNormal, -vPosToLight));

  float fAttTotal = ptLight.fConstantAtt + ptLight.fLinearAtt*fDist + ptLight.fExpAtt*fDist*fDist;

  return vec3(ptLight.vColor)*(ptLight.fAmbient+fDiffuse)/fAttTotal;
  //return vec3(ptLight.vColor)*(ptLight.fAmbient+fDiffuse);
}

void main() {
  vec4 colorOut;
  if(frag_textureId < 0){
    colorOut = texture(textureArray, vec3(texcoord.x, texcoord.z, -frag_textureId));
  }else{
    colorOut = texture(textureArray, vec3(texcoord.x + texcoord.z, 16 - texcoord.y, frag_textureId));
  }
  vec3 lightColor = vec3(0,0,0);
  for(int i = 0;i<POINT_LIGHT_COUNT;++i){
    lightColor+=getPointLightColor(pointLights[i], positionFrag, normal);
  }
  gl_FragColor = clamp(vec4(lightColor, 1.0), 0.0, 1.0) * colorOut;
}