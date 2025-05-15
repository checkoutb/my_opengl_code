#version 460 core


out vec4 frg_col;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;


struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;


struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];





in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

vec3 calDirLight(DirLight light, vec3 normal, vec3 viewDir);

vec3 calPointLight(PointLight light, vec3 normal, vec3 frgPos, vec3 viewDir);

void main() {

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
        result += calPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    
    frg_col = vec4(result, 1.0);
}

vec3 calDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 amb = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 dif = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 spe = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return (amb + dif + spe);
}



vec3 calPointLight(PointLight light, vec3 normal, vec3 frgPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - frgPos);

    float diff = max(dot(normal, lightDir), .0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - frgPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 amb = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 dif = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 spe = light.specular * spec * vec3(texture(material.specular, TexCoords));

    amb *= attenuation;
    dif *= attenuation;
    spe *= attenuation;

    return (amb + dif + spe);
}
