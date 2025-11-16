#version 450 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec2 texture_coord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 camerapos;

out vec3 position;
out vec3 normal;
out vec3 color;
out vec2 tex;
out vec3 eyevector;

void main() {
    // Transform the vertex position
    gl_Position = P * V * M * vec4(vertex_position, 1.0);

    position = (M * vec4(vertex_position, 1.0)).xyz;
    eyevector = vec3(0, 0, 0) - (V * M * vec4(vertex_position, 1.0)).xyz;
    color = vertex_color;
    tex = texture_coord;

    // normal = (V * M * vec4(vertex_normal, 0.0)).xyz;
    normal = mat3(transpose(inverse(M))) * vertex_normal;
}
