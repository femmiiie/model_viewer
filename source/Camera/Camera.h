#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
  void setPosSPH(glm::vec3 pos);
  glm::vec3 getPosSPH() { return this->positionSPH; }

  void setPosCAR(glm::vec3 pos);
  glm::vec3 getPosCAR() { return this->positionCAR; }

  void swapLookMode() { this->lookingAtOrigin = !this->lookingAtOrigin; };
  glm::mat4 getViewMatrix() { return this->viewMatrix; }

  void setViewDirection(glm::vec3 dir) { this->eyeVector = dir; }

  void convertSPHtoCAR();
  void convertCARtoSPH();

private:
  glm::vec3 positionSPH;
  glm::vec3 positionCAR;

  bool lookingAtOrigin;
  glm::vec3 eyeVector;

  glm::mat4 viewMatrix;


  void setViewMatrix();
};

#endif