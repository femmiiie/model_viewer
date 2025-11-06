#ifndef LIGHTDATA_H
#define LIGHTDATA_H

#include "LightObject.h"

#include <iostream>
#include <vector>
#include <numeric>


//Strictly an interface class to allow lights in shaders
struct LightData
{
  glm::vec4 position;
  glm::vec4 color;
  float power;
  float diffuse;
  float specular;
  float ambient;
};

#endif