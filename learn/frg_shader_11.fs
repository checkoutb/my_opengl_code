#version 460 core

// out vec4 frg_col;

// uniform vec3 objCol;
// uniform vec3 lightCol;

// void main() {
//    frg_col = vec4(lightCol * objCol, 1.0);
// }


// 12 basic lighting

in vec3 Normal;
in vec3 frg_pos;

out vec4 frg_col;

uniform vec3 objCol;
uniform vec3 lightCol;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - frg_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightCol;
    // vec3 diffuse = vec3(1.0, 0.0, 0.0);   // can let cube be red.

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - frg_pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightCol;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightCol;
    vec3 result = (ambient + diffuse + specular) * objCol;  // 环境光，漫反射光，镜面光
    frg_col = vec4(result, 1.0);
}

