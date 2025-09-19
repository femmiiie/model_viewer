#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

Renderer::Renderer(GLFWwindow* window) : gridObject(this->lights), axesObject(this->lights)
{
  this->prevTime = glfwGetTime();
  this->sinceLast = 0;
  this->frames = 0;

  this->window = window;
  this->renderGrid = false;
  this->renderAxes = false;

  glGenBuffers(1, &this->light_UBO);
}

void Renderer::timeStep()
{
  this->currTime = glfwGetTime();
  this->delta = this->currTime - this->prevTime;
  frames++;

  this->sinceLast += this->delta;
  if (this->sinceLast >= 1.000f)
  {
    std::cout << this->frames/this->sinceLast << " FPS" << std::endl;
    frames = 0;
    sinceLast = 0;
  }
}


void Renderer::render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (this->renderGrid) { this->gridObject.draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), this->cameraPosCAR); }
  if (this->renderAxes) { this->axesObject.draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), this->cameraPosCAR); }

  for (Object* object : this->rootObjects)
  {
    object->draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), this->cameraPosCAR);
  }
}


void Renderer::display()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  this->prevTime = this->currTime;
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Renderer::setCameraPosCAR(glm::vec3 pos)
{
  this->cameraPosCAR = pos;
  this->convertCARtoCYL();
  this->setViewMatrix();
}

void Renderer::setCameraPosCYL(glm::vec3 pos)
{
  this->cameraPosCYL = pos;
  this->convertCYLtoCAR();
  this->setViewMatrix();
}

void Renderer::convertCYLtoCAR()
{
  this->cameraPosCAR = glm::vec3(
    this->cameraPosCYL[0] * sin(this->cameraPosCYL[2]) * cos(this->cameraPosCYL[1]),
    this->cameraPosCYL[0] * cos(this->cameraPosCYL[2]),
    this->cameraPosCYL[0] * sin(this->cameraPosCYL[2]) * sin(this->cameraPosCYL[1])
  );
}


void Renderer::convertCARtoCYL() {

}

void Renderer::setViewMatrix()
{
  this->viewMatrix = glm::lookAt(
    this->cameraPosCAR, // Camera position
    glm::vec3(0.0f),    // Look at the origin
    glm::vec3(0, 1, 0)  // Head is looking up at the origin (set to 0,-1,0 to look upside-down)
  );
}

Object* Renderer::addMesh(std::string filepath, Object* parent)
{
  MeshObject* obj = new MeshObject(this->lights, filepath);
  if (parent) { parent->getChildren().emplace_back(obj); }
  else { this->rootObjects.emplace_back(obj); }

  return obj;
}

void Renderer::addLight(glm::vec3 position, glm::vec3 color)
{
  this->lights.emplace_back(Light(position, color));

  glBindBuffer(GL_UNIFORM_BUFFER, this->light_UBO);
  glBufferData(GL_UNIFORM_BUFFER, this->lights.size() * sizeof(Light), this->lights.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->light_UBO);
}