#version 330 core

layout(location=0) in vec3 coord3d;
layout(location=1) in float textureId;
layout(location=2) in float lightLevel;

flat out float textureId_geo;

void main() {
  gl_Position = vec4(coord3d, 1.0);
  textureId_geo = textureId;
}