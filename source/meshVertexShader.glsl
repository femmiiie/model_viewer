#version 330 core

// Input vertex attributes (from VBO)
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

// Uniforms
uniform mat4 M; // Model matrix
uniform mat4 V; // View matrix
uniform mat4 P; // Projection matrix

uniform vec3 light1pos;
uniform vec3 light2pos;
uniform vec3 camerapos;

out vec3 position;
out vec3 normal;
out vec3 eyevector;

out vec3 light1dir;
out vec3 light2dir;


void main() {
    // Transform the vertex position
    gl_Position = P * V * M * vec4(vertex_position, 1.0);

    position = (M * vec4(vertex_position, 1.0)).xyz;
    // eyevector = camerapos - (V * M * vec4(vertex_position, 1.0)).xyz;
    eyevector = vec3(0, 0, 0) - (V * M * vec4(vertex_position, 1.0)).xyz;


    // light1dir = (V * vec4(light1pos, 1.0)).xyz + eyevector;
    // light2dir = (V * vec4(light2pos, 1.0)).xyz + eyevector;

    light1dir = normalize(light1pos - position);
    light2dir = normalize(light2pos - position);

    // normal = (V * M * vec4(vertex_normal, 0.0)).xyz;
    normal = mat3(transpose(inverse(M))) * vertex_normal;
}
