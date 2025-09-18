// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "meshObject.hpp"
#include "gridObject.hpp"
#include "structs.h"
#include "util.h"

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

glm::mat4 projectionMatrix = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

int currSelected = 0;

// since base has both translation and rotation actions
// variable needed to keep track of which is currently being done
bool rotating = false;

int main()
{

    if (initWindow() != 0)
        return -1;


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // object initialization
    gridObject grid;

    meshObject base("../objects/base.obj");
    meshObject arm1("../objects/arm1.obj");
    meshObject arm1j("../objects/arm1j.obj"); // joint to allow arm1 to rotate without impacting base
    meshObject joint("../objects/joint.obj");
    meshObject arm2("../objects/arm2.obj");
    meshObject arm2j("../objects/arm2j.obj"); // joint to allow arm2 to rotate without impacting joint

    // hierarchy ordering
    base.children.push_back(&arm1j);
    arm1j.children.push_back(&arm1);
    arm1.children.push_back(&joint);
    joint.children.push_back(&arm2j);
    arm2j.children.push_back(&arm2);

    // object intial placement
    base.translate(glm::vec3(0.0f, 0.3f, 0.0f));
    arm1.translate(glm::vec3(0.8f, 0.8f, 0.0f));
    joint.translate(glm::vec3(1.3f, 0.0f, 0.0f));
    arm2.translate(glm::vec3(0.56218f, 0.0f, 0.0f));

    arm1.rotate(38.599f, glm::vec3(0.0f, 0.0f, 1.0f));
    arm2j.rotate(-51.401, glm::vec3(0.0f, 0.0f, 1.0f));
    arm2.rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 view_coords(8, 0.75f, 1.25f);

    light left_light(
        glm::vec3(0.0f, 3.0f, 5.0f),
        glm::vec3(0.7f, 0.443137f, 0.703922f));

    light right_light(
        glm::vec3(0.0f, 3.0f, -5.0f),
        glm::vec3(0.341176f, 0.333333f, 0.99607f));

    glfwSetWindowUserPointer(window, &view_coords);

    double lastTime = glfwGetTime();
    double sinceLast = 0;
    int frames = 0;
    do
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow();

        // ImGui::Begin("Model Viewer");
        if (ImGui::CollapsingHeader("Layout")) {
            static ImVec4 color(0.0f, 0.0f, 0.2f, 0.0f);
            ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview;
            if (ImGui::ColorPicker4("BG Color", (float*)&color, flags)) {
                glClearColor(color.x, color.y, color.z, color.w);
            }
        }



        // Timing (modified to use deltaTime)
        double currentTime = glfwGetTime();
        double delta = currentTime - lastTime;
        frames++;

        sinceLast += delta;
        if (sinceLast >= 1.000)
        {
            std::cout << frames/sinceLast << " fps" << std::endl;
            frames = 0;
            sinceLast = 0;
        }

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            currSelected = 0;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            if (currSelected == 0)
            {
                view_coords[1] += delta * (PI_DIV_2);
                if (view_coords[1] > PI)
                {
                    view_coords[1] -= 2 * PI;
                } // prevent any potential issues with uncapped growth
            }
            else if (currSelected == base.getId() && !rotating)
            {
                base.translate(glm::vec3(0.0f, 0.0f, -(delta * 4)));
            }
            else if (currSelected == base.getId())
            {
                base.rotate(delta * 50 * (PI_DIV_2), glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            if (currSelected == 0)
            {
                view_coords[1] -= delta * (PI_DIV_2);
                if (view_coords[1] < -PI)
                {
                    view_coords[1] += 2 * PI;
                } // prevent any potential issues with uncapped growth
            }
            else if (currSelected == base.getId() && !rotating)
            {
                base.translate(glm::vec3(0.0f, 0.0f, (delta * 4)));
            }
            else if (currSelected == base.getId())
            {
                base.rotate(delta * -50 * (PI_DIV_2), glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            if (currSelected == 0)
            {
                view_coords[2] -= delta * (PI_DIV_2);
                if (view_coords[2] < PI_DIV_32)
                {
                    view_coords[2] = PI_DIV_32;
                } // pretty clamping
            }
            else if (currSelected == base.getId() && !rotating)
            {
                base.translate(glm::vec3((delta * 4), 0.0f, 0.0f));
            }
            else if (currSelected == arm1.getId())
            {
                arm1j.rotate(delta * 50 * (PI_DIV_2), glm::vec3(0.0f, 0.0f, 1.0f));
            }
            else if (currSelected == arm2.getId())
            {
                arm2j.rotate(delta * 50 * (PI_DIV_2), glm::vec3(0.0f, 0.0f, 1.0f));
            }
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if (currSelected == 0)
            {
                view_coords[2] += delta * (PI_DIV_2);
                if (view_coords[2] > 31 * PI_DIV_32)
                {
                    view_coords[2] = 31 * PI_DIV_32;
                } // pretty clamping
            }
            else if (currSelected == base.getId() && !rotating)
            {
                base.translate(glm::vec3(-(delta * 4), 0.0f, 0.0f));
            }
            else if (currSelected == arm1.getId())
            {
                arm1j.rotate(delta * -50 * (PI_DIV_2), glm::vec3(0.0f, 0.0f, 1.0f));
            }
            else if (currSelected == arm2.getId())
            {
                arm2j.rotate(delta * -50 * (PI_DIV_2), glm::vec3(0.0f, 0.0f, 1.0f));
            }
        }


        //main loop was already obtuse enough as it is, so i moved these to util.h
        glm::vec3 pos = calc_pos(view_coords);
        glm::mat4 viewMatrix = calc_view_matrix(pos);

        selection_check(window, currSelected, rotating, base, arm1, arm2);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // DRAWING the SCENE
        grid.draw(viewMatrix, projectionMatrix);
        base.draw(viewMatrix, projectionMatrix, glm::mat4(1.0f), left_light, right_light, pos, currSelected);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        lastTime = currentTime;
        glfwSwapBuffers(window);
        glfwPollEvents();

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
    glm::vec3 *coords = static_cast<glm::vec3 *>(glfwGetWindowUserPointer(window));
    coords->x -= y;
    if (coords->x < 2) { coords->x = 2; }
}

static void refreshCallback(GLFWwindow *window)
{
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
    projectionMatrix = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
}