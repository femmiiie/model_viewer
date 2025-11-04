#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "../Camera/Camera.h"
#include "../Object/Object.h"
#include "../Object/MeshObject.h"
#include "../Object/GridObject.h"
#include "../Object/AxesObject.h"
#include "../Object/LightObject.h"


class Renderer
{
public:
  Renderer(GLFWwindow* window);

  void timeStep();

  void render();
  void display();

  Camera* getCamera() { return &this->camera; }

  void setCameraPosSPH(glm::vec3 pos) { this->camera.setPosSPH(pos); }
  void setCameraPosCAR(glm::vec3 pos) { this->camera.setPosCAR(pos); }

  const glm::mat4 getProjectionMatrix() { return this->projectionMatrix; }
  void setProjectionMatrix(const glm::mat4 p) { this->projectionMatrix = p; }

  std::vector<Object*> getRootObject() { return this->rootObjects; }
  Object* addMesh(std::string filepath, Object* parent = NULL);
  Object* addLight(Object* parent = NULL);

  bool& getRenderGrid_M() { return this->renderGrid; }
  bool& getRenderAxes_M() { return this->renderAxes; }
  bool& getRenderLightPoints_M() { return this->renderLightPoints; }

  GridObject& getGridObject_M() { return this->gridObject; }

  GLFWwindow* getWindow() { return this->window; }

private:
  double prevTime;
  double currTime;
  double sinceLast;
  double frames;
  double delta;

  bool renderGrid;
  bool renderAxes;
  bool renderLightPoints;

  GLFWwindow* window;

  Camera camera;
  
  glm::mat4 projectionMatrix;

  GridObject gridObject;
  AxesObject axesObject;
  std::vector<Object*> rootObjects;
  std::vector<LightData*> lights; //TODO: Implement removing lightsobjs to remove this light
  GLuint light_UBO;
};

#endif