#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "../Light.h"
#include "../Object/Object.h"
#include "../Object/MeshObject.h"
#include "../Object/GridObject.h"
#include "../Object/AxesObject.h"

class Renderer
{
public:
  Renderer(GLFWwindow* window);

  void timeStep();

  void render();
  void display();

  void setCameraPosCYL(glm::vec3 pos);
  void setCameraPosCAR(glm::vec3 pos);

  const glm::mat4 getProjectionMatrix() { return this->projectionMatrix; }
  void setProjectionMatrix(const glm::mat4 p) { this->projectionMatrix = p; }

  std::vector<MeshObject*> getRootObject() { return this->rootObjects; }
  Object* addMesh(std::string filepath, Object* parent = NULL);

  void addLight(glm::vec3 position, glm::vec3 color);



private:
  double prevTime;
  double currTime;
  double sinceLast;
  double frames;
  double delta;

  GLFWwindow* window;

  glm::vec3 cameraPosCYL;
  glm::vec3 cameraPosCAR;
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;

  GridObject gridObject;
  AxesObject axesObject;
  std::vector<MeshObject*> rootObjects;
  std::vector<Light> lights;
  GLuint light_UBO;

  bool renderGrid;
  bool renderAxes;
  bool renderLightPoints;

  void convertCYLtoCAR();
  void convertCARtoCYL();
  void setViewMatrix();
};

#endif