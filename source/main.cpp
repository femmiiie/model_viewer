// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Renderer/Renderer.h"
#include "Object/MeshObject.h"
#include "Object/GridObject.h"

#include <chrono>
#include <thread>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Precompute commonly used PI values
const float PI = glm::pi<float>();
const float PI_DIV_2 = PI / 2;
const float PI_DIV_32 = PI / 32;

// Function prototypes
int initWindow(void);
static void mouseCallback(GLFWwindow *, int, int, int);
static void scrollCallback(GLFWwindow *, double, double);
static void refreshCallback(GLFWwindow *);

GLint windowWidth = 1280, windowHeight = 720;
GLFWwindow *window;


int main()
{
    if (initWindow() != 0) { return -1; }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    int x_pos, y_pos, height, width;
    glfwGetWindowPos(window, &x_pos, &y_pos);
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width * 0.8f, height);

    Renderer renderer(window);
    renderer.setProjectionMatrix(glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f));
    renderer.addMesh("../objects/base.obj");

    // MeshObject base("../objects/base.obj");
    // MeshObject arm1("../objects/arm1.obj");
    // MeshObject arm1j("../objects/arm1j.obj"); // joint to allow arm1 to rotate without impacting base
    // MeshObject joint("../objects/joint.obj");
    // MeshObject arm2("../objects/arm2.obj");
    // MeshObject arm2j("../objects/arm2j.obj"); // joint to allow arm2 to rotate without impacting joint

    // hierarchy ordering
    // base.children.push_back(&arm1j);
    // arm1j.children.push_back(&arm1);
    // arm1.children.push_back(&joint);
    // joint.children.push_back(&arm2j);
    // arm2j.children.push_back(&arm2);

    // object intial placement
    // base.translate(glm::vec3(0.0f, 0.3f, 0.0f));
    // arm1.translate(glm::vec3(0.8f, 0.8f, 0.0f));
    // joint.translate(glm::vec3(1.3f, 0.0f, 0.0f));
    // arm2.translate(glm::vec3(0.56218f, 0.0f, 0.0f));

    // arm1.rotate(38.599f, glm::vec3(0.0f, 0.0f, 1.0f));
    // arm2j.rotate(-51.401, glm::vec3(0.0f, 0.0f, 1.0f));
    // arm2.rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    renderer.setCameraPosCAR({8.0f, 3.0f, 0.0f});
    renderer.addLight({0.0f, 3.0f, 5.0f}, {0.7f, 0.443f, 0.704f});
    renderer.addLight({0.0f, 3.0f, -5.0f}, {0.341f, 0.333f, 0.996f});

    glfwSetWindowUserPointer(window, &renderer);

    do
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Begin("Model Viewer");

        static ImVec2 menu_pos(width * 0.8f, 0);
        static ImVec2 menu_size(width * 0.2f, height);
        

        if (menu_pos.x != ImGui::GetWindowPos().x   ||
            menu_pos.y != ImGui::GetWindowPos().y   ||
            menu_size.x != ImGui::GetWindowSize().x ||
            menu_size.y != ImGui::GetWindowSize().y)
        {
            menu_size = ImVec2(ImGui::GetWindowSize().x, windowHeight);
            menu_pos = ImVec2(windowWidth - menu_size.x, 0);

            ImGui::SetWindowSize(menu_size);
            ImGui::SetWindowPos(menu_pos);

            glViewport(0, 0, windowWidth - menu_size.x, windowHeight);
            renderer.setProjectionMatrix(glm::perspective(45.0f, (float)(windowWidth - menu_size.x) / (float)windowHeight, 0.1f, 100.0f));
        }

        ImGui::GetWindowPos();
        ImGui::GetWindowSize();

        if (ImGui::CollapsingHeader("Layout")) {
            static ImVec4 color(0.0f, 0.0f, 0.2f, 0.0f);
            ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview;
            if (ImGui::ColorPicker4("BG Color", (float*)&color, flags))
            {
                std::cout << "update" << std::endl;
                glClearColor(color.x, color.y, color.z, color.w);
            }

            ImGui::Checkbox("Show grid", &renderer.getRenderGrid_M());
            ImGui::Checkbox("Show axes", &renderer.getRenderAxes_M());

            glm::vec3 pos = renderer.getCamera()->getPosCAR();
            static float vec[3] = {pos.x, pos.y, pos.z};
            if (ImGui::DragFloat3("Camera Position", vec, 0.1f))
            {
                renderer.getCamera()->setPosCAR({vec[0], vec[1], vec[2]});
            }
        }
        ImGui::End();

        // glfwPollEvents()
        // glfwGetKey()

        renderer.timeStep();
        renderer.render();
        renderer.display();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
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
    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetWindowRefreshCallback(window, refreshCallback);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    return 0;
}

static void mouseCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // std::cout << "Left mouse button pressed" << std::endl;
    }
}

static void scrollCallback(GLFWwindow *window, double x, double y)
{
    // glm::vec3 *coords = static_cast<glm::vec3 *>(glfwGetWindowUserPointer(window));
    // coords->x -= y;
    // if (coords->x < 2) { coords->x = 2; }
}

static void refreshCallback(GLFWwindow *window)
{
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);


    Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
    renderer->setProjectionMatrix(glm::perspective(45.0f, (float)windowWidth /*- ImGui::GetWindowSize().x*/ / (float)windowHeight, 0.1f, 100.0f));
}