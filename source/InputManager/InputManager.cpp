#include "InputManager.h"

void scrollCallback(GLFWwindow* window, double x, double y)
{
  Camera* cam = InputManager::renderer->getCamera();
  glm::vec3 curr_pos = cam->getPosSPH();
  cam->setPosSPH({curr_pos.x - (y * cam->getScrollScaling()), curr_pos.y, curr_pos.z});
}

void mycallback(int action, int mods)
{
  if (action == GLFW_PRESS) {
    InputManager::renderer->setCameraPosCAR({6.0f, 2.0f, 0.0f});
  }

  else if (action == GLFW_RELEASE) {
    InputManager::renderer->setCameraPosCAR({8.0f, 3.0f, 0.0f});
  }
 
  std::cout << "update!" << std::endl;
}


void moveCamera(int action, int mods) //TODO SWITCH TO SENSITIVITY VARIABLES OR GLOBAL
{
  static double last_x_pos, last_y_pos;
  double curr_x_pos, curr_y_pos;

  glfwGetCursorPos(InputManager::window, &curr_x_pos, &curr_y_pos);
  
  if (action == GLFW_REPEAT)
  {
    Camera* cam = InputManager::renderer->getCamera();
    glm::vec3 positionSPH = cam->getPosSPH();
    
    if (!(mods & GLFW_MOD_SHIFT)) //orbiting
    {
      glm::vec2 deltas((curr_x_pos - last_x_pos) * 0.005f, (curr_y_pos - last_y_pos) * 0.005f);
      positionSPH.y += deltas.x;
      positionSPH.z = glm::clamp(positionSPH.z - deltas.y, 0.0005f, glm::pi<float>() - 0.0005f);
      cam->setPosSPH(positionSPH);
    }

    else //panning
    {
      glm::vec3 positionCAR = cam->getPosCAR();
      glm::vec3 target = cam->getViewDirection(); 

      glm::vec3 forward = glm::normalize(positionCAR - target);
      glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
      glm::vec3 up = glm::normalize(glm::cross(forward, right));

      float scale = 0.0005f * positionSPH.x;

      glm::vec3 delta =
          (float)(last_x_pos - curr_x_pos) * right * scale +
          (float)(curr_y_pos - last_y_pos) * up * scale;

      cam->setPosCAR(positionCAR + delta);
      cam->setViewDirection(target + delta);
    }
  }

  last_x_pos = curr_x_pos;
  last_y_pos = curr_y_pos;
}


void InputManager::loadInputs(GLFWwindow* window, Renderer* renderer)
{
  InputManager::window = window;
  InputManager::renderer = renderer;

  glfwSetScrollCallback(window, scrollCallback);
  glfwSetMouseButtonCallback(window, InputManager::mousePress);
  glfwSetKeyCallback(window, InputManager::keyPress);

  InputManager::add(GLFW_KEY_A, moveCamera);
  // InputManager::add(GLFW_KEY_A, mycallback);

  InputManager::add(GLFW_MOUSE_BUTTON_MIDDLE, moveCamera);
};