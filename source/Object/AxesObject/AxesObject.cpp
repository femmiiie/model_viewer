#include "AxesObject.h"
#include "common/shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <numeric>

AxesObject::AxesObject() : Object()
{    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    std::vector<GLfloat> vertices =
    {
        0.0f,    0.001f,  0.0f,     1.0f, 0.0f, 0.0f,
        100.0f,  0.001f,  0.0f,     1.0f, 0.0f, 0.0f,

        0.0f,    0.0f,   0.0f,     0.0f, 1.0f, 0.0f,
        0.0f,    100.0f, 0.0f,     0.0f, 1.0f, 0.0f,

        0.0f,    0.01f,  0.0f,     0.0f, 0.0f, 1.0f,
        0.0f,    0.01f,  100.0f,   0.0f, 0.0f, 1.0f,
    };

    std::vector<GLuint> indices(vertices.size() / 6);
    std::iota(indices.begin(), indices.end(), 0);

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
    shaderProgram = ShaderLoader::Load(
        "axes.vs.glsl",
        NULL,
        NULL,
        "axes.gs.glsl",
        "grid.fs.glsl"
    );
    
    shaderProgram = LoadShadersGeneric("axes.vs.glsl", NULL, NULL, "axes.gs.glsl", "grid.fs.glsl");
    
}


void AxesObject::draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera) 
{
    glUseProgram(shaderProgram);
    
    glm::mat4 MVP = projection * view * modelMatrix;
    GLuint matrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(MVP));

    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    GLuint viewportID = glGetUniformLocation(shaderProgram, "invViewportSize");
    glUniform2f(viewportID, 1.0f / (float)vp[2], 1.0f / (float)vp[3]);

    GLuint thicknessID = glGetUniformLocation(shaderProgram, "thickness");
    glUniform1f(thicknessID, 4);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}