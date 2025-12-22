#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Settings/Settings.h"

class Camera
{
public:
  void setPosSPH(glm::vec3 pos);
  glm::vec3 getPosSPH() { return this->positionSPH; }

  void setPosCAR(glm::vec3 pos);
  glm::vec3 getPosCAR() { return this->positionCAR; }

  void swapLookMode() { this->lookingAtOrigin = !this->lookingAtOrigin; };
  glm::mat4 getViewMatrix() { return this->viewMatrix; }

  void setScrollScaling(float scroll) { this->scrollScaling = scroll; }
  float getScrollScaling() { return this->scrollScaling; }

  void setViewDirection(glm::vec3 dir) { this->eyeVector = dir; }
  glm::vec3 getViewDirection() { return this->eyeVector; }

  Settings::ProjectionType& getProjectionType_M() { return this->type; }

  float& getFOV_M() { return this->fov; }
  float& getAspect_M() { return this->aspectRatio; }
  float& getNearClip_M() { return this->nearClip; }
  float& getFarClip_M() { return this->farClip; }

  void deferUpdate() { this->needsUpdate = true; }
  bool requiresUpdate() { return this->needsUpdate; }
  void update();

  void convertSPHtoCAR();
  void convertCARtoSPH();

private:
  float scrollScaling;
  bool needsUpdate;

  glm::vec3 positionSPH;
  glm::vec3 positionCAR;

  bool lookingAtOrigin;
  glm::vec3 eyeVector = glm::vec3(0.0f);

  glm::mat4 viewMatrix;


  void setViewMatrix();

  Settings::ProjectionType type = Settings::PERSPEC;
  float aspectRatio = 16.0f / 9.0f;
  float fov = 45.0f;
  float nearClip = 0.1f;
  float farClip = 100.0f;
};

#endif