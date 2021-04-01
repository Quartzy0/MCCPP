#version 330 core

in vec4 texcoord;
flat in float frag_textureId;
in vec3 normal;
in vec3 positionFrag;

uniform sampler2DArray textureArray;
uniform sampler3D lightLevel;

void main() {
  vec4 colorOut;
  if(frag_textureId < 0){
    colorOut = texture(textureArray, vec3(texcoord.x, texcoord.z, -frag_textureId));
  }else{
    colorOut = texture(textureArray, vec3(texcoord.x + texcoord.z, 16 - texcoord.y, frag_textureId));
  }
  vec3 doneBoi = colorOut.xyz * texture(lightLevel, trunc(positionFrag)).x;
  gl_FragColor = vec4(doneBoi.xyz, colorOut.w);
}