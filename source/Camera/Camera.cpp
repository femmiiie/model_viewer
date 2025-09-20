#include "Camera.h"

void Camera::setPosCAR(glm::vec3 pos)
{
  this->positionCAR = pos;
  this->convertCARtoSPH();
}

void Camera::setPosSPH(glm::vec3 pos)
{
  this->positionSPH = pos;
  this->convertSPHtoCAR();
}


void Camera::convertSPHtoCAR()
{
  this->positionCAR = glm::vec3(
    this->positionSPH[0] * sin(this->positionSPH[2]) * cos(this->positionSPH[1]),
    this->positionSPH[0] * cos(this->positionSPH[2]),
    this->positionSPH[0] * sin(this->positionSPH[2]) * sin(this->positionSPH[1])
  );
  this->setViewMatrix();
}


void Camera::convertCARtoSPH() {
  this->setViewMatrix();
}

void Camera::setViewMatrix()
{
  this->viewMatrix = glm::lookAt(
    this->positionCAR,  // Camera position
    glm::vec3(0.0f),    // Look at the origin
    glm::vec3(0, 1, 0)  // Head is looking up at the origin (set to 0,-1,0 to look upside-down)
  );
}