#version 330 core

layout(location=0) in vec3 coord3d;
layout(location=1) in float textureId;

flat out float textureId_geo;

//out VS_OUT{
//  float textureId;
//} vs_out;

void main() {
  gl_Position = vec4(coord3d, 1.0);
  textureId_geo = textureId;
}