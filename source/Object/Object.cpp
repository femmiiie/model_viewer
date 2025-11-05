#include "Object.h"

Object::Object() : modelMatrix(glm::mat4(1)) 
{
    // Generate and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

Object::~Object()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void Object::generateModelMatrix()
{
    glm::mat4 matrix(1);
    matrix = glm::translate(matrix, this->position);

    matrix = glm::rotate(matrix, this->rotation.x, {1, 0, 0});
    matrix = glm::rotate(matrix, this->rotation.y, {0, 1, 0});
    matrix = glm::rotate(matrix, this->rotation.z, {0, 0, 1});

    matrix = glm::scale(matrix, this->scaling);

    this->modelMatrix = matrix;
}

void Object::translate(const glm::vec3& translation) 
{
    // Apply translation to the model matrix
    modelMatrix = glm::translate(modelMatrix, translation);
}

void Object::rotate(float angle, const glm::vec3& axis) 
{
    // Apply rotation to the model matrix
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
}

void Object::scale(float factor)
{
  
}

glm::mat4 Object::getTransformation()
{
    if (!children.empty()) { return modelMatrix * children[0]->getTransformation(); }
    return modelMatrix;
}