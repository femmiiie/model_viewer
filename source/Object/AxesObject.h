#ifndef AXESOBJECT_H
#define AXESOBJECT_H

#include "Object.h"

class AxesObject : public Object
{
public:
  AxesObject();
  ~AxesObject() {}

  void draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera);
};

#endif