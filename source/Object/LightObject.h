#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H

#include "Object.h"

#include "common/shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <numeric>

struct LightData
{
  glm::vec4 lightPosition;
  glm::vec4 lightColor;
};


class LightObject : public Object
{
public:
  LightObject();
  ~LightObject() {}

  void draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera);

  LightData light;
};

#endif