// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <nfd.h>
#include <nfd_glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "InputManager/InputManager.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"

#include "Renderer/Renderer.h"

#include <algorithm>

// Function prototypes
int initWindow(void);
static void refreshCallback(GLFWwindow *);

static constexpr float baseImGuiScale        = 1.0f;
static const ImVec2 defaultWindowSize        = {1920.f, 1080.f};
static constexpr float defaultMenuWidthRatio = 0.2f;
static float prevScaling                     = baseImGuiScale;

GLint windowWidth = 1280, windowHeight = 720;
GLFWwindow *window;

static bool updateMenu       = false;
static ImVec2 menuFuturePos  = {};
static ImVec2 menuFutureSize = {};

int main()
{
  if (initWindow() != 0) return 1;
  Settings::Load("./mv_config.dat");

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsDark();

  NFD_Init();

  ImGui::GetStyle().ScaleAllSizes(baseImGuiScale);
  io.FontGlobalScale = baseImGuiScale;
  prevScaling        = baseImGuiScale;

  int x_pos, y_pos, height, width;
  glfwGetWindowPos(window, &x_pos, &y_pos);
  glfwGetWindowSize(window, &width, &height);
  glViewport(0, 0, windowWidth - (width * 0.2f), windowHeight);

  Renderer renderer(window);
  renderer.setProjectionMatrix(
      glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f));

  Menu::Init(&renderer);
  InputManager::loadInputs(window, &renderer);

  renderer.setCameraPosCAR({8.0f, 3.0f, 0.0f});

  glfwSetWindowUserPointer(window, &renderer);

  do
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (updateMenu)
    {
      ImGui::SetNextWindowPos(menuFuturePos);
      ImGui::SetNextWindowSize(menuFutureSize);
      updateMenu = false;
    }

    ImGui::Begin("Model Viewer");

    static ImVec2 menu_pos(width * 0.8f, 0);
    static ImVec2 menu_size(width * 0.2f, height);

    if (menu_pos.x != ImGui::GetWindowPos().x || menu_pos.y != ImGui::GetWindowPos().y ||
        menu_size.x != ImGui::GetWindowSize().x || menu_size.y != ImGui::GetWindowSize().y)
    {
      menu_size = ImVec2(ImGui::GetWindowSize().x, windowHeight);
      menu_pos  = ImVec2(windowWidth - menu_size.x, 0);

      ImGui::SetWindowSize(menu_size);
      ImGui::SetWindowPos(menu_pos);

      glViewport(0, 0, windowWidth - menu_size.x, windowHeight);
      renderer.setProjectionMatrix(glm::perspective(
          45.0f, (float)(windowWidth - menu_size.x) / (float)windowHeight, 0.1f, 100.0f));
    }

    ImGui::GetWindowPos();
    ImGui::GetWindowSize();

    if (ImGui::CollapsingHeader("Objects", ImGuiTreeNodeFlags_DefaultOpen))
    {
      Menu::ObjectSelector();
      Menu::ObjectModifier();
    }

    if (ImGui::CollapsingHeader("Settings"))
    {
      Menu::Settings();
    }
    ImGui::End();

    InputManager::pollInputs();

    renderer.update();
    renderer.render();
    renderer.display();

  } // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  NFD_Quit();

  glfwTerminate();
  Settings::Save("./mv_config.dat");
  return 0;
}

// Initialize GLFW and create a window
int initWindow()
{
  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // FOR MAC

  window = glfwCreateWindow(windowWidth, windowHeight, "Model Viewer", NULL, NULL);
  if (window == NULL)
  {
    fprintf(stderr, "Failed to open GLFW window.\n");
    glfwTerminate();
    return -1;
  }
  glfwMaximizeWindow(window);
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  // Set up inputs
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
  glfwSetCursorPos(window, (double)windowWidth / 2, (double)windowHeight / 2);
  glfwSetWindowRefreshCallback(window, refreshCallback);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  return 0;
}

static void refreshCallback(GLFWwindow *window)
{
  glfwGetWindowSize(window, &windowWidth, &windowHeight);

  const float widthRatio  = static_cast<float>(windowWidth) / defaultWindowSize.x;
  const float heightRatio = static_cast<float>(windowHeight) / defaultWindowSize.y;
  const float ratio       = std::max(widthRatio, heightRatio);

  float targetScale = baseImGuiScale * ratio;
  if (targetScale < 1.2f) targetScale = 1.2f;
  if (targetScale > 3.0f) targetScale = 3.0f;

  if (ImGui::GetCurrentContext() != nullptr && prevScaling > 0.0f)
  {
    const float scaleDelta = targetScale / prevScaling;
    if (scaleDelta > 1.01f || scaleDelta < 0.99f)
    {
      ImGui::GetStyle().ScaleAllSizes(scaleDelta);
      ImGui::GetIO().FontGlobalScale *= scaleDelta;
      prevScaling = targetScale;
    }
  }

  const float defaultMenuWidth = defaultWindowSize.x * defaultMenuWidthRatio;
  float scaledMenuWidth        = defaultMenuWidth * ratio;

  const float maxMenuWidth = static_cast<float>(windowWidth) * 0.5f;
  const float minMenuWidth = std::min(320.0f, maxMenuWidth);
  if (scaledMenuWidth < minMenuWidth) scaledMenuWidth = minMenuWidth;
  if (scaledMenuWidth > maxMenuWidth) scaledMenuWidth = maxMenuWidth;

  const int menuWidthPx = static_cast<int>(scaledMenuWidth);
  int renderWidth       = windowWidth - menuWidthPx;
  if (renderWidth < 1) renderWidth = 1;

  menuFutureSize = ImVec2(static_cast<float>(menuWidthPx), static_cast<float>(windowHeight));
  menuFuturePos  = ImVec2(static_cast<float>(windowWidth - menuWidthPx), 0.0f);
  updateMenu     = true;

  glViewport(0, 0, renderWidth, std::max(windowHeight, 1));

  if (Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window)))
  {
    renderer->setProjectionMatrix(glm::perspective(
        45.0f, static_cast<float>(renderWidth) / static_cast<float>(std::max(windowHeight, 1)),
        0.1f, 100.0f));
  }
}