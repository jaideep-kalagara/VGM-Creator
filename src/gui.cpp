#include "gui.h"

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <memory>

#include "app.h"
#include "imgui_style.h"
#include "log.h"

Gui::~Gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Gui::Gui(GLFWwindow* window, VGMLogger logger)
    : m_window(window), m_logger(logger) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/consola.ttf", 14.0f);

    setupImGuiStyle();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    m_editor = std::make_unique<Editor>(m_window, m_logger);
}

void Gui::update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(m_window, GLFW_TRUE);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window")) {
            for (auto& window : m_WindowStates) {
                if (ImGui::MenuItem(window.first.c_str(), nullptr, window.second)) {
                    window.second = !window.second;
                }
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    if (m_WindowStates["Main Info"]) {
        if (ImGui::Begin("VGM Creator", &m_WindowStates["Main Info"], ImGuiWindowFlags_NoResize)) {
            ImGui::Text("Welcome to VGM Creator!");
            ImGui::Separator();
            ImGui::Text("Version: %s", VGM_VERSION);
        }
        ImGui::End();
    }

    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    if (m_WindowStates["Credits"]) {
        if (ImGui::Begin("Credits", &m_WindowStates["Credits"], ImGuiWindowFlags_NoResize)) {
            ImGui::Text("Developed by Jaideep Kalagara :>");
            ImGui::Separator();
            ImGui::Text("Using Dear ImGui, GLFW, GLAD, NFD,");
            ImGui::Text("miniz, nlohmann_json, and miniaudio.");
        }
        ImGui::End();
    }

    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    if (m_WindowStates["Editor"]) {
        if (ImGui::Begin("Editor", &m_WindowStates["Editor"], ImGuiWindowFlags_NoResize)) {
            m_editor->getGui();
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
