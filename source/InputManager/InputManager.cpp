#include "InputManager.h"

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

void InputManager::loadInputs(GLFWwindow* window, Renderer* renderer)
{
  InputManager::window = window;
  InputManager::renderer = renderer;

  glfwSetMouseButtonCallback(window, InputManager::mousePress);
  glfwSetKeyCallback(window, InputManager::keyPress);

  InputManager::add(GLFW_KEY_A, mycallback);
};