#pragma once

#include <imgui.h>
#include <miniz.h>

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "editor.h"
#include "instrument.h"

static bool saveJsonSettings(Instrument& inst, VGMLogger& logger, const nlohmann::json& json) {
    namespace fs = std::filesystem;

    if (!fs::exists(inst.info.path))
        return false;

    std::ifstream file(inst.info.path, std::ios::binary);
    std::vector<unsigned char> zipData((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());
    file.close();

    mz_zip_archive inZip{};
    if (!mz_zip_reader_init_mem(&inZip, zipData.data(), zipData.size(), 0))
        return false;

    int fileCount = mz_zip_reader_get_num_files(&inZip);

    mz_zip_archive outZip{};
    if (!mz_zip_writer_init_heap(&outZip, 0, 0))
        return false;

    for (int i = 0; i < fileCount; i++) {
        mz_zip_archive_file_stat st;
        mz_zip_reader_file_stat(&inZip, i, &st);

        std::string name = st.m_filename;

        if (name.size() >= 5 && name.substr(name.size() - 5) == ".json")
            continue;

        mz_zip_writer_add_from_zip_reader(&outZip, &inZip, i);
    }

    std::string jsonText = json.dump(4);
    if (!mz_zip_writer_add_mem(&outZip,
                               "instrument_config.json",
                               jsonText.data(),
                               jsonText.size(),
                               MZ_BEST_COMPRESSION)) {
        mz_zip_writer_end(&outZip);
        mz_zip_reader_end(&inZip);
        return false;
    }

    void* finalZip = nullptr;
    size_t finalSize = 0;
    mz_zip_writer_finalize_heap_archive(&outZip, &finalZip, &finalSize);

    std::ofstream out(inst.info.path, std::ios::binary);
    out.write((char*)finalZip, finalSize);
    out.close();

    mz_free(finalZip);
    mz_zip_writer_end(&outZip);
    mz_zip_reader_end(&inZip);

    return true;
}

inline void EditorInstrumentPopup(Editor& editor) {
    EditorInfo info = editor.getEditorInfo();

    auto& instruments = *info.instruments;
    int& index = *info.currentInstrumentIndex;
    VGMLogger& logger = *info.logger;

    if (!ImGui::BeginPopup("InstrumentPopup"))
        return;

    if (index >= 0 && index < (int)instruments.size()) {
        Instrument& inst = instruments[index];

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
                0.0f, 1.0f,
                "%.2f",
                ImGuiSliderFlags_AlwaysClamp);
        }
    } else {
        ImGui::Text("No instrument selected.");
    }

    if (ImGui::Button("Save Settings")) {
        if (index >= 0 && index < (int)instruments.size()) {
            Instrument& inst = instruments[index];

            logger.log(LOG_LEVEL_INFO,
                       "Saving settings for instrument: %s",
                       inst.info.name.c_str());

            nlohmann::json json;
            json["Name"] = inst.info.name;
            json["Description"] = inst.info.description;
            json["Samples"] = inst.info.rawSampleData;
            json["Settings"] = inst.info.settings;

            if (!saveJsonSettings(inst, logger, json)) {
                logger.log(LOG_LEVEL_ERROR,
                           "Failed to save settings for instrument: %s",
                           inst.info.name.c_str());
            }
        }
    }

    if (ImGui::Button("Close"))
        ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
}
