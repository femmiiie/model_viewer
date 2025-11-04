#include "MeshObject.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "common/shader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"


MeshObject::MeshObject(std::vector<LightData*>& scene_lights, std::string filepath) : Object(), scene_lights(scene_lights)
{ 
    glBindVertexArray(VAO);

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filepath)) {
        printf("Error parsing file");
        exit(1);
    }

    std::vector<tinyobj::shape_t> shapes = reader.GetShapes();
    tinyobj::attrib_t attrib = reader.GetAttrib();

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> normals;

    for (const auto& shape : shapes) 
    {
        for (const auto& index : shape.mesh.indices) 
        {
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            normals.push_back(attrib.normals[3 * index.normal_index + 0]);
            normals.push_back(attrib.normals[3 * index.normal_index + 1]);
            normals.push_back(attrib.normals[3 * index.normal_index + 2]);

            indices.push_back(indices.size());
        }
    }
    
    numIndices = (GLsizei)indices.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    GLuint normalbuff;
    glGenBuffers(1, &normalbuff);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuff);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normalbuff);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load and compile shaders
    shaderProgram = LoadShaders("mesh.vs.glsl", "mesh.fs.glsl");
}

MeshObject::~MeshObject() {}

void MeshObject::draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera) 
{
    glUseProgram(shaderProgram);


    glm::mat4 M = transform * modelMatrix;
    GLuint MID = glGetUniformLocation(shaderProgram, "M");
    glUniformMatrix4fv(MID, 1, GL_FALSE, glm::value_ptr(M));

    GLuint VID = glGetUniformLocation(shaderProgram, "V");
    glUniformMatrix4fv(VID, 1, GL_FALSE, glm::value_ptr(view));

    GLuint PID = glGetUniformLocation(shaderProgram, "P");
    glUniformMatrix4fv(PID, 1, GL_FALSE, glm::value_ptr(projection));
    
    glUniform1i(glGetUniformLocation(shaderProgram, "num_lights"), this->scene_lights.size());

    GLuint cameraID = glGetUniformLocation(shaderProgram, "camerapos");
    glUniform3fv(cameraID, 1, glm::value_ptr(camera));

    // Draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    for (Object* child : this->children)
    {
        child->draw(view, projection, M, camera);
    }
}






