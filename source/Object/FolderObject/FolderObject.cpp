#include "FolderObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <numeric>

FolderObject::FolderObject() : Object() { name = "New Folder"; }


void FolderObject::draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera) 
{
  for (Object* child : this->children)
  {
    child->draw(view, projection, modelMatrix * transform, camera);
  }
}