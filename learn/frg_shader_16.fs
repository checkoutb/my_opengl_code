#version 460 core


out vec4 frg_col;


struct Material {
    // vec3 ambient;
    // vec3 diffuse;
    sampler2D diffuse;
    
    // vec3 specular;
    sampler2D specular;
    
    float shininess;
};
uniform Material material;


struct DirLight {
    vec3 dir;
    vec3 amb;
    vec3 dif;
    vec3 spe;
};
uniform DirLight dirLight;


struct PointLight {
    vec3 pos;

    float constant;
    float linear;
    float quadratic;

    vec3 amb;
    vec3 dif;
    vec3 spe;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];





in vec3 Normal;
in vec3 frg_pos;
in vec2 myTex;

uniform vec3 viewPos;

vec3 calDirLight(DirLight light, vec3 normal, vec3 viewDir);

vec3 calPointLight(PointLight light, vec3 normal, vec3 frgPos, vec3 viewDir);

void main() {

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - frg_pos);

    vec3 result = calDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
        result += calPointLight(pointLights[i], norm, frg_pos, viewDir);
    }

    
    frg_col = vec4(result, 1.0);
}

vec3 calDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.dir);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 amb = light.amb * vec3(texture(material.diffuse, myTex));
    vec3 dif = light.dif * diff * vec3(texture(material.diffuse, myTex));
    vec3 spe = light.spe * spec * vec3(texture(material.specular, myTex));

    return (amb + dif + spe);
}



vec3 calPointLight(PointLight light, vec3 normal, vec3 frgPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.pos - frgPos);

    float diff = max(dot(normal, lightDir), .0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.pos - frgPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 amb = light.amb * vec3(texture(material.diffuse, myTex));
    vec3 dif = light.dif * diff * vec3(texture(material.diffuse, myTex));
    vec3 spe = light.spe * spec * vec3(texture(material.specular, myTex));

    amb *= attenuation;
    dif *= attenuation;
    spe *= attenuation;

    return (amb + dif + spe);
}
