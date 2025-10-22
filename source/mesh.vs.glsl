#version 450 core

// Input vertex attributes (from VBO)
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

// Uniforms
uniform mat4 M; // Model matrix
uniform mat4 V; // View matrix
uniform mat4 P; // Projection matrix

uniform vec3 camerapos;

out vec3 position;
out vec3 normal;
out vec3 eyevector;

void main() {
    // Transform the vertex position
    gl_Position = P * V * M * vec4(vertex_position, 1.0);

    position = (M * vec4(vertex_position, 1.0)).xyz;
    eyevector = vec3(0, 0, 0) - (V * M * vec4(vertex_position, 1.0)).xyz;

    // normal = (V * M * vec4(vertex_normal, 0.0)).xyz;
    normal = mat3(transpose(inverse(M))) * vertex_normal;
}
