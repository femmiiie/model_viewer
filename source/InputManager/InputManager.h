#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <set>
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
  static inline std::set<int> heldKeys;
  static inline std::set<int> heldMouseButtons;
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

    if (action == GLFW_PRESS) { heldMouseButtons.insert(key); }
    else if (action == GLFW_RELEASE) { heldMouseButtons.erase(key); }

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

  static int pollModifier(int left_mod, int mod)
  {
    if (glfwGetKey(window, left_mod) == GLFW_PRESS || 
        glfwGetKey(window, left_mod + 4) == GLFW_PRESS)
    {
      return mod;
    }

    return 0;
  }

  static int pollModifiers()
  {
    int mods = 0;
    mods |= pollModifier(GLFW_KEY_LEFT_SHIFT,   GLFW_MOD_SHIFT);
    mods |= pollModifier(GLFW_KEY_LEFT_CONTROL, GLFW_MOD_CONTROL);
    mods |= pollModifier(GLFW_KEY_LEFT_ALT,     GLFW_MOD_ALT);
    mods |= pollModifier(GLFW_KEY_LEFT_SUPER,   GLFW_MOD_SUPER);
    return mods;
  }

  static void pollInputs()
  {
    int mods = pollModifiers();

    for (int key : heldKeys)
    {
      if (glfwGetKey(window, key) == GLFW_PRESS && callbacks.count(key))
      {
        for (auto& callback : callbacks[key]) { callback(GLFW_REPEAT, mods); }
      }
    }

    for (int button : heldMouseButtons)
    {
      if (glfwGetMouseButton(window, button) == GLFW_PRESS && callbacks.count(button))
      {
        for (auto& callback : callbacks[button]) { callback(GLFW_REPEAT, mods); }
      }
    }
  }

private:
  InputManager() {}

};

#endif