#ifndef LIGHTDATA_H
#define LIGHTDATA_H

#include "LightObject.h"

#include <iostream>
#include <vector>
#include <numeric>


//Strictly an interface class to allow lights in shaders
struct LightData
{
  static LightData* CreateInterface(LightObject& obj)
  {
    return new LightData({
      &obj.position,
      &obj.color,
      &obj.power,
      &obj.diffuse,
      &obj.specular,
      &obj.ambient
    });
}

  glm::vec3* lightPosition;
  glm::vec4* lightColor;
  float* lightPower;
  float* lightDiffuse;
  float* lightSpecular;
  float* lightAmbient;
};

#endif