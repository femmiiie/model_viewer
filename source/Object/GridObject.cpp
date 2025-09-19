#include "GridObject.h"
#include "common/shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

GridObject::GridObject(std::vector<Light>& scene_lights) : Object(scene_lights)
{    
    glBindVertexArray(VAO);

    std::vector<GLfloat> vertices =
    {
        //edges
        -5.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,
        -5.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,

        //lines parallel to z axis
        -4.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        -4.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,

        -3.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        -3.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,

        -2.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        -2.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,
        
        -1.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        -1.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,
        
        0.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
        
        1.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,
        
        2.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        2.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,
        
        3.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        3.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,
        
        4.0f, 0.0f, -5.0f,   1.0f, 1.0f, 1.0f,
        4.0f, 0.0f, 5.0f,   1.0f, 1.0f, 1.0f,

        //lines parallel to x axis        
        -5.0f, 0.0f, -4.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f, -4.0f,   1.0f, 1.0f, 1.0f,
        
        -5.0f, 0.0f, -3.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f, -3.0f,   1.0f, 1.0f, 1.0f,

        -5.0f, 0.0f, -2.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f, -2.0f,   1.0f, 1.0f, 1.0f,

        -5.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,

        -5.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,

        -5.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,

        -5.0f, 0.0f, 2.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f, 2.0f,   1.0f, 1.0f, 1.0f,

        -5.0f, 0.0f, 3.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f, 3.0f,   1.0f, 1.0f, 1.0f,

        -5.0f, 0.0f, 4.0f,   1.0f, 1.0f, 1.0f,
        5.0f, 0.0f, 4.0f,   1.0f, 1.0f, 1.0f,

        //axes
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        5.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.0f, 5.0f, 0.0f,   0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 5.0f,   0.0f, 0.0f, 1.0f,
    };

    std::vector<GLuint> indices = {
        //edges
        0, 1,
        1, 2,
        2, 3,
        3, 0,

        //lines parallel to x axis
        4, 5,
        6, 7,
        8, 9,
        10, 11,
        12, 13,
        14, 15,
        16, 17,
        18, 19,
        20, 21,

        22, 23,
        24, 25,
        26, 27,
        28, 29,
        30, 31,
        32, 33,
        34, 35,
        36, 37,
        38, 39,

        //axes
        40, 41,
        42, 43,
        44, 45
    };
    
    numIndices = (GLsizei)indices.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load and compile shaders
    shaderProgram = LoadShaders("gridVertexShader.glsl", "gridFragmentShader.glsl");
}

GridObject::~GridObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
}

void GridObject::draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera) 
{
    glUseProgram(shaderProgram);
    
    glm::mat4 MVP = projection * view * modelMatrix;
    GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));
    
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
