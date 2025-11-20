// editor.cpp

#include "editor.h"

#include <GLAD/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <nfd.hpp>
#include <vector>

#include "log.h"

void Editor::getGui() {
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
}

void Editor::addMusicalInstrument() {
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