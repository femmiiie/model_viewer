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
  glm::mat4 viewMatrix = this->camera.getViewMatrix();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (this->renderGrid) { this->gridObject.draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), this->camera.getPosCAR()); }
  if (this->renderAxes) { this->axesObject.draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), this->camera.getPosCAR()); }

  for (Object* object : this->rootObjects)
  {
    object->draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), this->camera.getPosCAR());
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

void Renderer::setCameraPosCAR(glm::vec3 pos) { this->camera.setPosCAR(pos); }
void Renderer::setCameraPosSPH(glm::vec3 pos) { this->camera.setPosSPH(pos); }





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