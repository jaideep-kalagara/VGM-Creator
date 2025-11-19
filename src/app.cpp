// app.cpp
#include "app.h"

#define VGM_VERSION "0.0.1"
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void App::init() {
    m_logger = VGMLogger();

    if (!glfwInit()) {
        m_logger.log(LOG_LEVEL_ERROR, "Failed to initialize GLFW");
        return;
    }

    m_logger.log(LOG_LEVEL_INFO, "VGM Creator version %s", VGM_VERSION);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(800, 600, "VGM Creator", nullptr, nullptr);
    if (!m_window) {
        m_logger.log(LOG_LEVEL_ERROR, "Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        m_logger.log(LOG_LEVEL_ERROR, "Failed to initialize GLAD");
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return;
    }

    m_logger.log(LOG_LEVEL_INFO, "OpenGL version loaded: %s", glGetString(GL_VERSION));

    initializeImGui();
}

void App::run() {
    while (shouldRun()) {
        glClearColor(0.133f, 0.412f, 0.612f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        updateImGui();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void App::terminate() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
    m_logger.log(LOG_LEVEL_INFO, "Application terminated");
}

void App::initializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/consola.ttf", 14.0f);

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void App::updateImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // create menu bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Main Info")) {
                m_WindowStates["Main Info"] = !m_WindowStates["Main Info"];
                m_logger.log(LOG_LEVEL_DEBUG, "Showing main info: %s", m_WindowStates["Main Info"] ? "true" : "false");
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    // create main window
    ImGui::SetNextWindowSize(ImVec2(200, 80), ImGuiCond_Always);
    if (m_WindowStates["Main Info"]) {
        if (ImGui::Begin("VGM Creator", &m_WindowStates["Main Info"], ImGuiWindowFlags_NoResize)) {
            ImGui::Text("Welcome to VGM Creator!");
            ImGui::Text("Version: %s", VGM_VERSION);
        }
        ImGui::End();
    }
}
