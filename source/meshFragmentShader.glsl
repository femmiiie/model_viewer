#version 330 core

in vec3 position;
in vec3 normal;
in vec3 eyevector;

in vec3 light1dir;
in vec3 light2dir;

uniform vec3 light1pos;
uniform vec3 light1color;

uniform vec3 light2pos;
uniform vec3 light2color;

uniform vec3 camerapos;

uniform bool is_selected;

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
    float amb_str = 0.1;
    if (is_selected) { amb_str += 0.1; }
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
    //TODO: P1bTask4 - Find a way to draw the selected part in a brighter color.
    vec3 light1 = comp_light(light1pos, light1color, light1dir);
    vec3 light2 = comp_light(light2pos, light2color, light2dir);

    color = vec4(clamp(light1 + light2, 0, 1), 1.0);

    // color = vec4(clamp(normal, 0, 1), 1.0); 

}

