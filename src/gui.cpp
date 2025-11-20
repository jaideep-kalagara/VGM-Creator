// gui.cpp
#include "gui.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <nfd.hpp>
#include <vector>

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
};

void Gui::update() {
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
            for (auto& window : m_WindowStates) {
                if (ImGui::MenuItem(window.first.c_str())) {
                    window.second = !window.second;
                }
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    // Main window
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
            ImGui::Text("minizip-ng, nlohmann_json, and miniaudio.");
        }
        ImGui::End();
    }

    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    if (m_WindowStates["Editor"]) {
        if (ImGui::Begin("Editor", &m_WindowStates["Editor"], ImGuiWindowFlags_NoResize)) {
            if (ImGui::BeginListBox("Instruments")) {
                if (ImGui::BeginPopup("InstrumentPopup")) {
                    if (m_currentInstrumentIndex >= 0 &&
                        m_currentInstrumentIndex < (int)m_musicalInstruments.size()) {
                        ImGui::Text("Config for instrument: %s",
                                    m_musicalInstruments[m_currentInstrumentIndex].c_str());
                    } else {
                        ImGui::Text("No instrument selected.");
                    }

                    if (ImGui::Button("Close"))
                        ImGui::CloseCurrentPopup();

                    ImGui::EndPopup();
                }
                for (size_t i = 0; i < m_musicalInstruments.size(); i++) {
                    const std::string& item = m_musicalInstruments[i];
                    bool isSelected = (i == m_currentInstrumentIndex);

                    ImGui::Selectable(item.c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick);
                    if (ImGui::IsItemClicked())
                        m_currentInstrumentIndex = static_cast<int>(i);

                    // Handle double-click
                    if (ImGui::IsItemHovered() &&
                        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        ImGui::OpenPopup("InstrumentPopup");
                        m_currentInstrumentIndex = (int)i;
                        m_logger.log(LOG_LEVEL_INFO,
                                     "Double-clicked instrument: %s", item.c_str());
                    }
                }
                ImGui::EndListBox();
            }

            if (ImGui::Button("Play Sample")) {
                if (m_currentInstrumentIndex >= 0) {
                    m_logger.log(LOG_LEVEL_INFO,
                                 "Playing sample for instrument: %s",
                                 m_musicalInstruments[m_currentInstrumentIndex].c_str());
                } else {
                    m_logger.log(LOG_LEVEL_WARN, "No instrument selected to play sample");
                }
            }

            if (ImGui::Button("Add Instrument")) {
                addMusicalInstrument();
            }

            ImGui::End();  // End Editor
        }
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::addMusicalInstrument() {
    NFD::UniquePathU8 outPath;

    const nfdu8filteritem_t filters[1] = {{"Instrument Files", "zip"}};

    nfdresult_t result = NFD::OpenDialog(outPath, filters, 1, "C:/");

    if (result == NFD_OKAY) {
        std::string instrumentPath =
            reinterpret_cast<const char*>(outPath.get());

        m_musicalInstruments.push_back(
            instrumentPath.substr(instrumentPath.find_last_of("/\\") + 1));

        m_logger.log(LOG_LEVEL_INFO,
                     "Added instrument from path: %s", instrumentPath.c_str());
    } else if (result == NFD_CANCEL) {
        m_logger.log(LOG_LEVEL_INFO, "User canceled instrument selection.");
    } else {
        m_logger.log(LOG_LEVEL_ERROR, "NFD Error: %s", NFD::GetError());
    }
}
