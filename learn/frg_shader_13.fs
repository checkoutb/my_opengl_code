#version 460 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;


struct Light {
    vec3 pos;
    vec3 amb;
    vec3 dif;
    vec3 spe;
};

uniform Light light;

in vec3 Normal;
in vec3 frg_pos;

out vec4 frg_col;

uniform vec3 objCol;
uniform vec3 lightCol;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 ambient = light.amb * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - frg_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.dif * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - frg_pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.spe * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    frg_col = vec4(result, 1.0);
}

