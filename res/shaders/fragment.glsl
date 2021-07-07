#version 430
#extension GL_ARB_shader_storage_buffer_object : enable


#ifndef RENDER_DISTANCE
#define RENDER_DISTANCE 20
#endif
#ifndef VERTICAL_SIZE
#define VERTICAL_SIZE 16
#endif
#define XZ_BLOCKS RENDER_DISTANCE*16
#define Y_BLOCKS VERTICAL_SIZE*16

in vec4 texcoord;
flat in float frag_textureId;
in vec3 normal;
in vec3 positionFrag;

uniform sampler2DArray textureArray;
uniform sampler3D lightLevel;

layout(std430, binding = 1) readonly buffer lightLevels{
  int levels[];
};

void main() {
  vec4 colorOut;
  if(frag_textureId < 0){
    colorOut = texture(textureArray, vec3(texcoord.x, texcoord.z, -frag_textureId));
  }else{
    colorOut = texture(textureArray, vec3(texcoord.x + texcoord.z, 16 - texcoord.y, frag_textureId));
  }
//  float lightUp = texture(lightLevel, trunc(vec3(positionFrag.x, positionFrag.y+1, positionFrag.z))).x*255;
//  float lightNorth = texture(lightLevel, trunc(vec3(positionFrag.x, positionFrag.y, positionFrag.z+1))).x*255;
//  float lightSouth = texture(lightLevel, trunc(vec3(positionFrag.x, positionFrag.y, positionFrag.z-1))).x*255;
//  float lightWest = texture(lightLevel, trunc(vec3(positionFrag.x+1, positionFrag.y, positionFrag.z))).x*255;
//  float lightEast = texture(lightLevel, trunc(vec3(positionFrag.x-1, positionFrag.y, positionFrag.z))).x*255;
//
//  float finalLight = max(lightUp, max(lightNorth, max(lightSouth, max(lightWest, lightEast))));

//  vec3 doneBoi = vec3(1,1,1) * (texture(lightLevel, positionFrag/16.0f).x);
//  int i = int(trunc(positionFrag.y))*Y_BLOCKS*Y_BLOCKS+int(trunc(positionFrag.z))*XZ_BLOCKS+int(trunc(positionFrag.x));
//  float lightLevel = float(levels[i]) / 15.0f;
//  gl_FragColor = vec4(lightLevel, lightLevel, lightLevel, 1.0) * colorOut;

  gl_FragColor = colorOut;
}