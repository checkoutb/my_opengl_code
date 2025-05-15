#pragma once

#include "my_shader.h"
#include "my_mesh.h"

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <iostream>

std::vector<Texture> textures_loaded;

class My_Model {
public:
    My_Model (char* path) {
        loadModel(path);
    }
    void draw(MyShader& shader);
private:
    std::vector<My_Mesh> meshes;
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    My_Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

void My_Model::draw(MyShader& shader) {
    for (int i = 0; i < meshes.size(); ++i) {
        meshes[i].draw(shader);
    }
}


void My_Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout<<"ERRORL: assimp: "<<import.GetErrorString()<<std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}


void My_Model::processNode(aiNode* node, const aiScene* scene) {
    std::cout<<"processNode "<<node->mNumMeshes<<", "<<node->mNumChildren<<std::endl;
    for (int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(this->processMesh(mesh, scene));
    }

    for (int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

My_Mesh My_Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;

        glm::vec3 vc;
        vc.x = mesh->mVertices[i].x;
        vc.y = mesh->mVertices[i].y;
        vc.z = mesh->mVertices[i].z;
        vertex.position = vc;

        vc.x = mesh->mNormals[i].x;
        vc.y = mesh->mNormals[i].y;
        vc.z = mesh->mNormals[i].z;
     
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        } else {
            vertex.texCoords = glm::vec2(.0f, 0.f);
        }

        vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMap = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());
        std::vector<Texture> speMap = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), speMap.begin(), speMap.end());
    }

    return My_Mesh(vertices, indices, textures);
}



unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false) {
    using namespace std;
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

std::vector<Texture> My_Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for (int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool fnd = false;
        for (int j = 0; j < textures_loaded.size(); ++j) {
            if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                std::cout<<"INFO: loadMaterialtextures: cached\n";
                fnd = true;
                break;
            }
        }

        if (!fnd) {
            std::cout<<"INFO: loadMaterialtextures: load\n";
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
        
    }
    return textures;
}




