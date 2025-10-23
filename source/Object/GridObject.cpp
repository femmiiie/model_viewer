#include "GridObject.h"
#include "common/shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <numeric>

GridObject::GridObject(std::vector<Light>& scene_lights) : Object(scene_lights)
{   
    this->gridSize = 10;
    this->gridSpacing = 1;
    this->generateGrid();

    // Load and compile shaders
    shaderProgram = LoadShaders("grid.vs.glsl", "grid.fs.glsl");
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
    glDrawElements(GL_LINES, this->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void GridObject::generateGrid()
{
    if (this->gridSize < 1) { this->gridSize = 1; }
    if (this->gridSpacing < 1) { this->gridSpacing = 1; }


    int step = this->gridSize / this->gridSpacing;
    float halfGrid = (float)(gridSize/2);

    this->vertices.clear();
    this->vertices.reserve(6 * 2 * step);

    for (int i = -halfGrid; i <= halfGrid; i += gridSpacing)
    {
        this->vertices.insert(this->vertices.end(), {
            (float)i, -0.001f, -halfGrid, 1.0f, 1.0f, 1.0f,
            (float)i, -0.001f,  halfGrid, 1.0f, 1.0f, 1.0f
        });

        this->vertices.insert(this->vertices.end(), {
            -halfGrid, 0.001f, (float)i, 1.0f, 1.0f, 1.0f,
             halfGrid, 0.001f, (float)i, 1.0f, 1.0f, 1.0f
        });
    }

    this->indices.clear();
    this->indices.resize(this->vertices.size() / 6);

    std::iota(this->indices.begin(), this->indices.end(), 0);

    glBindVertexArray(VAO);
    
    this->numIndices = (GLsizei)this->indices.size();
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(GLfloat), this->vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}