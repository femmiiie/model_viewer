//adapted from https://stackoverflow.com/questions/54686818/glsl-geometry-shader-to-replace-gllinewidth%E2%80%8B
#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in vec3 geomColor[];
out vec3 fragColor;

uniform mat4 MVP;
uniform vec2 invViewportSize;
uniform float thickness;

void main()
{
    fragColor = geomColor[0];
    float r = thickness;

    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;

    vec2 dir    = normalize((p2.xy/p2.w - p1.xy/p1.w) / invViewportSize);
    vec2 offset = vec2(-dir.y, dir.x) * thickness * invViewportSize;

    gl_Position = p1 + vec4(offset.xy * p1.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 - vec4(offset.xy * p1.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p2 + vec4(offset.xy * p2.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p2 - vec4(offset.xy * p2.w, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}