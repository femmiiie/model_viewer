#include "Camera.h"

void Camera::setPosCAR(glm::vec3 pos)
{
  this->positionCAR = pos;
  this->convertCARtoSPH();
}

void Camera::setPosSPH(glm::vec3 pos)
{
  this->positionSPH = pos;
  if (this->positionSPH.x < 2) { this->positionSPH.x = 2; }
  this->convertSPHtoCAR();
}


void Camera::convertSPHtoCAR()
{
  float r = this->positionSPH.x;
  float theta = this->positionSPH.y;
  float phi = this->positionSPH.z;

  this->positionCAR = glm::vec3(
    r * sin(phi) * cos(theta),
    r * cos(phi),
    r * sin(phi) * sin(theta)
  );

  this->positionCAR += this->eyeVector;
  this->setViewMatrix();
}


void Camera::convertCARtoSPH() {
  glm::vec3 offset = this->positionCAR - this->eyeVector;

  float r = glm::length(offset);
  float theta = std::atan2(offset.z, offset.x);
  float phi = std::acos(offset.y / r);

  this->positionSPH = glm::vec3(r, theta, phi);
  this->setViewMatrix();
}

void Camera::setViewMatrix()
{
  this->viewMatrix = glm::lookAt(
    this->positionCAR,
    this->eyeVector,
    glm::vec3(0, 1, 0)
  );
}