#include "Object.h"


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