#include "Object.h"

Object::Object(std::vector<Light>& lights) : modelMatrix(glm::mat4(1)), scene_lights(lights) 
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