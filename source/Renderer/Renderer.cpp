#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

Renderer::Renderer(GLFWwindow* window) : gridObject(), axesObject()
{
  this->prevTime = glfwGetTime();
  this->sinceLast = 0;
  this->frames = 0;

  this->window = window;

  this->camera.setScrollScaling(0.5);

  glGenBuffers(1, &this->light_UBO);
}

Renderer::~Renderer()
{

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
  if (Settings::ShowGrid.active) { this->gridObject.draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), this->camera.getPosCAR()); }
  if (Settings::ShowAxes.active) { this->axesObject.draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), this->camera.getPosCAR()); }

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
 

MeshObject* Renderer::addMesh(std::string filepath, Object* parent)
{
  MeshObject* obj = new MeshObject(this->lights, filepath);
  if (parent) { parent->getChildren_M().emplace_back(obj); }
  else { this->rootObjects.emplace_back(obj); }

  return obj;
}

LightObject* Renderer::addLight(Object* parent)
{
  LightObject* obj = new LightObject();

  if (parent) { parent->getChildren_M().emplace_back(obj); }
  else { this->rootObjects.emplace_back(obj); }

  this->lights.emplace_back(obj);
  this->regenerateLightUBO();

  return obj;
}

LightObject* Renderer::addLight(LightData data, Object* parent)
{
  LightObject* obj = new LightObject();
  obj->setLightingData(data);

  if (parent) { parent->getChildren_M().emplace_back(obj); }
  else { this->rootObjects.emplace_back(obj); }

  this->lights.emplace_back(obj);
  this->regenerateLightUBO();

  return obj;
}

void Renderer::regenerateLightUBO()
{
  std::vector<LightData> lightData;
  lightData.reserve(this->lights.size());

  for (LightObject* light : this->lights)
  {
    LightData data = {  
      .position = glm::vec4(light->getPosition_M(), 0),
      .color    = light->getColor(),
      .power    = light->getPower(),
      .diffuse  = light->getDiffuse(),
      .specular = light->getSpecular(),
      .ambient  = light->getAmbient()
    };

    lightData.emplace_back(data);
  }

  glBindBuffer(GL_UNIFORM_BUFFER, this->light_UBO);
  glBufferData(GL_UNIFORM_BUFFER, lightData.size() * sizeof(LightData), lightData.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->light_UBO);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}