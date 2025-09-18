#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

Renderer::Renderer(GLFWwindow* window)
{
  this->prevTime = glfwGetTime();
  this->sinceLast = 0;
  this->frames = 0;

  this->window = window;
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
  this->cameraPosCAR = this->calcPos();
  glm::mat4 viewMatrix = this->calcViewMatrix(this->cameraPosCAR);

  if (this->renderGrid) { this->gridObject.draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), this->cameraPosCAR); }
  if (this->renderAxes) { this->axesObject.draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), this->cameraPosCAR); }

  for (Object* object : this->rootObject->getChildren())
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

glm::vec3 Renderer::calcPos()
{
  return glm::vec3(
    this->cameraPosCYL[0] * sin(this->cameraPosCYL[2]) * cos(this->cameraPosCYL[1]),
    this->cameraPosCYL[0] * cos(this->cameraPosCYL[2]),
    this->cameraPosCYL[0] * sin(this->cameraPosCYL[2]) * sin(this->cameraPosCYL[1])
  );
}


glm::mat4 Renderer::calcViewMatrix(glm::vec3 pos)
{
  return glm::lookAt(
    pos,               // Camera position
    glm::vec3(0.0f),   // Look at the origin
    glm::vec3(0, 1, 0) // Head is looking up at the origin (set to 0,-1,0 to look upside-down)
  );
}