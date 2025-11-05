#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H

#include "../Object.h"

#include "common/shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <numeric>

struct LightData;

class LightObject : public Object
{
public:
  LightObject();
  ~LightObject() {}

  void setPosition(glm::vec3 position) { this->position = position; }
  void setColor(glm::vec4 color) { this->color = color; }
  void setLightingData(LightData& data);

  void draw(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& transform, const glm::vec3& camera);
  void update();

  //override lights to be non-rotatable/scalable
  void rotate(float angle, const glm::vec3& axis) {}
  void scale(float factor) {}

  glm::vec4& getColor() { return color; }
  float& getPower() { return power; }
  float& getDiffuse() { return diffuse; }
  float& getSpecular() { return specular; }
  float& getAmbient() { return ambient; }

private:
  glm::vec4 color = glm::vec4(1.0f);

  float power    = 30.0f;
  float diffuse  = 1.0f;
  float specular = 1.0f;
  float ambient  = 1.0f;

  friend struct LightData;
};

#endif