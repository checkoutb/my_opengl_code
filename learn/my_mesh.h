#pragma once


#include "my_shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <stddef.h>  // offsetof

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    aiString path;
};

class My_Mesh {
    
public:

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    My_Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(MyShader& shader);

 private:
    unsigned int vao, vbo, ebo;
    void setupMesh();
};

My_Mesh::My_Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}


void My_Mesh::setupMesh() {
    unsigned int vtx_sz = sizeof(Vertex);
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * vtx_sz, &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vtx_sz, (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vtx_sz, (void*) offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vtx_sz, (void*) offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}


void My_Mesh::draw(MyShader& shader) {
    /* uniform sampler2D texture_diffuse1; */
    /* uniform sampler2D texture_diffuse2; */
    /* uniform sampler2D texture_diffuse3; */
    /* uniform sampler2D texture_specular1; */
    /* uniform sampler2D texture_specular2; */

    unsigned int difNr = 1;
    unsigned int speNr = 1;

    for (int i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(difNr++);
        } else if (name == "texture_specular") {
            number = std::to_string(speNr++);
        }
        shader.use();
        glUniform1i(glGetUniformLocation(shader.getId(), ("material." + name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


