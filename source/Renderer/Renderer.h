#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "MeshObject.h"
#include "GridObject.h"
#include "AxesObject.h"

class Renderer
{
public:
  struct Light
  {
    Light(glm::vec3 pos, glm::vec3 color) : pos(pos), color(color) {};
  
    glm::vec3 pos;
    glm::vec3 color;
  };

  Renderer(GLFWwindow* window);

  void timeStep();

  void render();
  void display();

private:
  double prevTime;
  double currTime;
  double sinceLast;
  double frames;
  double delta;

  GLFWwindow* window;

  glm::vec3 cameraPosCYL;
  glm::vec3 cameraPosCAR;
  glm::mat4 projectionMatrix;

  Object* rootObject;
  GridObject gridObject;
  AxesObject axesObject;
  std::vector<Light> lights;

  bool renderGrid;
  bool renderAxes;
  bool renderLightPoints;

  glm::vec3 calcPos();
  glm::mat4 calcViewMatrix(glm::vec3 pos);

  const glm::mat4 getProjectionMatrix() { return this->projectionMatrix; }
  void setProjectionMatrix(const glm::mat4 p) { this->projectionMatrix = p; }

};

#endif