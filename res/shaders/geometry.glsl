#version 430
#extension GL_EXT_geometry_shader4 : enable

layout (lines) in;
layout (triangle_strip, max_vertices = 6) out;

flat in float textureId_geo[];

out vec4 texcoord;
flat out float frag_textureId;
out vec3 normal;
out vec3 positionFrag;

uniform mat4 vp;
uniform mat4 m;

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

    // Emit the vertices of the quad
    frag_textureId = textureId_geo[0];

    normal = normalize(cross(a.xyz - b.xyz, b.xyz - c.xyz));

    texcoord = a; gl_Position = vp * m * vec4(a.xyz, 1); positionFrag = (m * vec4(a.xyz, 1.0)).xyz; EmitVertex();
    texcoord = d; gl_Position = vp * m * vec4(d.xyz, 1); positionFrag = (m * vec4(d.xyz, 1.0)).xyz; EmitVertex();
    texcoord = c; gl_Position = vp * m * vec4(c.xyz, 1); positionFrag = (m * vec4(c.xyz, 1.0)).xyz; EmitVertex();

    texcoord = a; gl_Position = vp * m * vec4(a.xyz, 1); positionFrag = (m * vec4(a.xyz, 1.0)).xyz; EmitVertex();
    texcoord = d; gl_Position = vp * m * vec4(d.xyz, 1); positionFrag = (m * vec4(d.xyz, 1.0)).xyz; EmitVertex();
    texcoord = b; gl_Position = vp * m * vec4(b.xyz, 1); positionFrag = (m * vec4(b.xyz, 1.0)).xyz; EmitVertex();
    EndPrimitive();
}
