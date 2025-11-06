#ifndef FOLDEROBJECT_H
#define FOLDEROBJECT_H

#include "../Object.h"

class FolderObject : public Object
{
public:
  FolderObject();
  ~FolderObject() {}

  void draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera);
  void update() { generateModelMatrix(); }
};

#endif