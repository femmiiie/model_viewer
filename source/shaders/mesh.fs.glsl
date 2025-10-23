#version 450 core

in vec3 position;
in vec3 normal;
in vec3 eyevector;


struct Light
{
  vec4 position;
  vec4 color;
};

layout(std140, binding = 0) uniform Lights
{
    Light lights[128];
};

uniform int num_lights;

uniform mat4 V;
uniform vec3 camerapos;

// Output color
out vec4 color;

vec3 comp_light(vec3 lightpos, vec3 lightcolor, vec3 lightdir) {
    vec3 mat_color = vec3(0.9, 0.9, 0.9);
    // vec3 mat_color = normal;
    vec3 diff_color = mat_color;
    vec3 amb_color = mat_color;
    vec3 spec_color = mat_color * 0.1;

    float power = 30;
    float dist = length(lightpos - position);

//diffuse calculations
    vec3 n = normalize(normal);
    vec3 l = normalize(lightdir);
    vec3 diffuse = diff_color * lightcolor * power * clamp(dot(n, l), 0, 1) / (dist * dist);
    // vec3 diffuse = diff_color * lightcolor * power * clamp(dot(n, l), 0, 1) / (dist);


//ambient calculations
    float amb_str = 0.0;
    vec3 ambient = amb_str * amb_color;

//specular calculations
    vec3 e = normalize(eyevector);
    vec3 r = reflect(-l, n);

    vec3 specular = spec_color * lightcolor * power * pow(clamp(dot(e, r), 0, 1), 10) / (dist*dist);

//combination
    return diffuse + ambient + specular;
}

void main() {
    // Set the fragment color
    vec3 sum_of_lights = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < num_lights; i++)
    {
        // vec3 lightDirection = normalize(lights[i].position.xyz - position);
        vec3 lightDirection = (V * lights[i].position).xyz + eyevector;
        sum_of_lights += comp_light(lights[i].position.xyz, lights[i].color.xyz, lightDirection);
    }

    color = vec4(clamp(sum_of_lights,0, 1), 1.0);
}

