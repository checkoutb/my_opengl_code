#version 460 core

struct Material {
    // vec3 ambient;
    // vec3 diffuse;
    sampler2D diffuse;
    
    // vec3 specular;
    sampler2D specular;
    
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
in vec2 myTex;

out vec4 frg_col;

uniform vec3 objCol;
uniform vec3 lightCol;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    // vec3 ambient = light.amb * material.ambient;
//    vec3 ambient = light.amb * vec3(texture(material.diffuse, myTex));
    vec3 ambient = light.amb * texture(material.diffuse, myTex).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - frg_pos);
    float diff = max(dot(norm, lightDir), 0.0);

    // vec3 diffuse = light.dif * (diff * material.diffuse);
//    vec3 diffuse = light.dif * diff * vec3(texture(material.diffuse, myTex));
    vec3 diffuse = light.dif * diff * texture(material.diffuse, myTex).rgb;

    vec3 viewDir = normalize(viewPos - frg_pos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.spe * (spec * material.specular);
//    vec3 specular = light.spe * spec * vec3(texture(material.specular, myTex));
    vec3 specular = light.spe * spec * texture(material.specular, myTex).rgb;
    
    vec3 result = ambient + diffuse + specular;
    frg_col = vec4(result, 1.0);
}



// out vec4 FragColor;

// struct Material {
//     sampler2D diffuse;
//     sampler2D specular;    
//     float shininess;
// }; 

// struct Light {
//     vec3 position;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

// in vec3 FragPos;  
// in vec3 Normal;  
// in vec2 TexCoords;
  
// uniform vec3 viewPos;
// uniform Material material;
// uniform Light light;

// void main()
// {
//     // ambient
//     vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
//     // diffuse 
//     vec3 norm = normalize(Normal);
//     vec3 lightDir = normalize(light.position - FragPos);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
//     // specular
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);  
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
//     vec3 result = ambient + diffuse + specular;
//     FragColor = vec4(result, 1.0);
// } 


