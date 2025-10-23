#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <map>
#include <functional>

#include "../Renderer/Renderer.h"

using Callback = std::function<void(int, int)>;

class InputManager
{
public:
  static inline std::map<int , std::vector<Callback>> callbacks;
  static inline GLFWwindow* window;
  static inline Renderer* renderer;

  static void loadInputs(GLFWwindow* window, Renderer* renderer);
  static void add(int key, Callback callback) { callbacks[key].emplace_back(callback); }

  static void mousePress(GLFWwindow* window, int button, int action, int mods) 
  {
    if (callbacks.find(button) == callbacks.end()) { return; }
    for (Callback& callback : callbacks[button]) { callback(action, mods); }
  }

  static void keyPress(GLFWwindow* window, int button, int scancode, int action, int mods) 
  {
    if (callbacks.find(button) == callbacks.end()) { return; }
    for (Callback& callback : callbacks[button]) { callback(action, mods); }
  }

private:
  InputManager() {}

};

void loadInputs();

#endif