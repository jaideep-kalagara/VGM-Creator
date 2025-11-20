#pragma once

#include <imgui.h>

#include "editor.h"
#include "instrument.h"

inline void EditorInstrumentPopup(Editor& editor) {
    EditorInfo info = editor.getEditorInfo();

    auto& instruments = *info.instruments;
    int& currentIndex = *info.currentInstrumentIndex;
    VGMLogger& logger = *info.logger;

    if (ImGui::BeginPopup("InstrumentPopup")) {
        if (currentIndex >= 0 && currentIndex < static_cast<int>(instruments.size())) {
            Instrument& inst = instruments[currentIndex];

            ImGui::Text("%s", inst.info.name.c_str());
            ImGui::Separator();
            ImGui::Text("Description: %s", inst.info.description.c_str());
            ImGui::Text("Path: %s", inst.info.path.c_str());
            ImGui::Separator();
            ImGui::Text("Settings:");
            for (auto& setting : inst.info.settings) {
                ImGui::SliderFloat(
                    setting.first.c_str(),
                    &setting.second,
                    0.0f,
                    1.0f,
                    "%.2f",
                    ImGuiSliderFlags_AlwaysClamp);
            }
        } else {
            ImGui::Text("No instrument selected.");
        }

        if (ImGui::Button("Save Settings")) {
            if (currentIndex >= 0 && currentIndex < static_cast<int>(instruments.size())) {
                logger.log(
                    LOG_LEVEL_INFO,
                    "Saving settings for instrument: %s",
                    instruments[currentIndex].info.name.c_str());
            }
        }

        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
