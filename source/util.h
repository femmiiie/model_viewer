#ifndef _UTIL_H
#define _UTIL_H

#include <map>
#include <random>
#include <GLFW/glfw3.h>
#include "meshObject.hpp"

std::string getRandomPlatonic();
void selection_check(GLFWwindow* window, int& curr, bool& r, meshObject& base, meshObject& arm1, meshObject& arm2);
glm::mat4 calc_view_matrix(glm::vec3 view_coords);
int getPickedId(GLFWwindow *window);


std::map<int, std::string> platonics =
{
 {1, "../objects/tetrahedron.obj"},
 {2, "../objects/hexahedron.obj"},
 {3, "../objects/octahedron.obj"},
 {4, "../objects/dodecahedron.obj"},
 {5, "../objects/icosahedron.obj"}
};


std::string get_random_platonic() 
{
  static std::random_device rd;
  static std::mt19937 mt(rd());
  std::uniform_int_distribution<> r(1, platonics.size());

  int randomIndex = r(mt);
  return platonics[randomIndex];
}



void selection_check(GLFWwindow* window, int& curr, bool& r, meshObject& base, meshObject& arm1, meshObject& arm2)
{
  if (glfwGetKey(window, GLFW_KEY_T))
  {
      curr = base.getId();
      r = false;
  }

  else if (glfwGetKey(window, GLFW_KEY_R))
  {
      curr = base.getId();
      r = true;
  }

  else if (glfwGetKey(window, GLFW_KEY_1) || glfwGetKey(window, GLFW_KEY_KP_1))
  {
      curr = arm1.getId();
  }

  else if (glfwGetKey(window, GLFW_KEY_2) || glfwGetKey(window, GLFW_KEY_KP_2))
  {
      curr = arm2.getId();
  }
}


glm::vec3 calc_pos(glm::vec3 view_coords)
{
  return glm::vec3(
    view_coords[0] * sin(view_coords[2]) * cos(view_coords[1]),
    view_coords[0] * cos(view_coords[2]),
    view_coords[0] * sin(view_coords[2]) * sin(view_coords[1])
  );
}


glm::mat4 calc_view_matrix(glm::vec3 pos)
{
  return glm::lookAt(
    pos,               // Camera position
    glm::vec3(0.0f),   // Look at the origin
    glm::vec3(0, 1, 0) // Head is looking up at the origin (set to 0,-1,0 to look upside-down)
  );
}


int getPickedId(GLFWwindow *window)
{
    glFlush();
    // --- Wait until all the pending drawing commands are really done.
    // Ultra-mega-over slow !
    // There are usually a long time between glDrawElements() and
    // all the fragments completely rasterized.
    glFinish();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned char data[4];

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    std::pair<int, int> frame, win;
    glfwGetFramebufferSize(window, &frame.first, &frame.second);
    glfwGetWindowSize(window, &win.first, &win.second);

    x = x * (frame.first / win.first);
    y = (win.second - y) * (frame.second / win.second);
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

    int pickedId = data[0];
    return pickedId;
}

#endif