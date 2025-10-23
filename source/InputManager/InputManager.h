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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using Callback = std::function<void(int, int)>;

class InputManager
{
public:
  static inline std::map<int , std::vector<Callback>> callbacks;
  static inline GLFWwindow* window;
  static inline Renderer* renderer;

  static void loadInputs(GLFWwindow* window, Renderer* renderer);
  static void add(int key, Callback callback) { callbacks[key].emplace_back(callback); }

  static void mousePress(GLFWwindow* window, int key, int action, int mods) 
  {
    ImGui_ImplGlfw_MouseButtonCallback(window, key, action, mods);

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) { return; }

    if (callbacks.find(key) == callbacks.end()) { return; }
    for (Callback& callback : callbacks[key]) { callback(action, mods); }
  }

  static void keyPress(GLFWwindow* window, int key, int scancode, int action, int mods) 
  {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard) { return; }

    if (callbacks.find(key) == callbacks.end()) { return; }
    for (Callback& callback : callbacks[key]) { callback(action, mods); }
  }

private:
  InputManager() {}

};

#endif