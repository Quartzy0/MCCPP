#version 330 core
#extension GL_EXT_geometry_shader4 : enable

layout (lines) in;
layout (triangle_strip, max_vertices = 6) out;

flat in float textureId_geo[];

out vec4 texcoord;
flat out float frag_textureId;

uniform mat4 mvp;

vec4 whenEq(vec4 x, vec4 y){
    return 1.0 - abs(sign(x-y));
}

void main() {
    // Two input vertices will be the first and last vertex of the quad
    vec4 a = gl_in[0].gl_Position;
    vec4 d = gl_in[1].gl_Position;

    // Calculate the middle two vertices of the quad
    vec4 b = a;
    vec4 c = a;

    if(a.y == d.y) { // y same
        c.z = d.z;
        b.x = d.x;
    } else { // x or z same
        b.y = d.y;
        c.xz = d.xz;
    }

    //  Janovo sranje tukaj
    // Fancy if
//    float ay_is_not_dy = abs(sign(a.y - d.y));
//    b.x = d.x * (1.0 - ay_is_not_dy) + b.x * ay_is_not_dy;
//    b.y = d.y * ay_is_not_dy + b.y * (1.0 - ay_is_not_dy);
//    c.x = c.x * (1.0 - ay_is_not_dy) + ay_is_not_dy * d.x;
//    c.z = d.z;

    //  Konec Janovega sranja

    // Emit the vertices of the quad
    frag_textureId = textureId_geo[0];

    texcoord = a; gl_Position = mvp * vec4(a.xyz, 1); EmitVertex();
    texcoord = c; gl_Position = mvp * vec4(c.xyz, 1); EmitVertex();
    texcoord = d; gl_Position = mvp * vec4(d.xyz, 1); EmitVertex();

    texcoord = d; gl_Position = mvp * vec4(d.xyz, 1); EmitVertex();
    texcoord = b; gl_Position = mvp * vec4(b.xyz, 1); EmitVertex();
    texcoord = a; gl_Position = mvp * vec4(a.xyz, 1); EmitVertex();
    EndPrimitive();
}
