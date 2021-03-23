#version 330 core

in vec4 texcoord;
flat in float frag_textureId;

uniform sampler2DArray textureArray;

void main() {
  if(frag_textureId < 0){
    gl_FragColor = texture(textureArray, vec3(texcoord.x, texcoord.z, -frag_textureId));
  }else{
    gl_FragColor = texture(textureArray, vec3(texcoord.x + texcoord.z, 16 - texcoord.y, frag_textureId));
  }
  //  Janovo sranje tukaj
//  float negaitve_id = float(frag_textureId > 0);
//  gl_FragColor = texture(textureArray, vec3(texcoord.x + negaitve_id * texcoord.z, (1.0 - negaitve_id) * texcoord.z + negaitve_id * (16 - texcoord.y), abs(frag_textureId)));
  //  Konec Janovega sranja

}