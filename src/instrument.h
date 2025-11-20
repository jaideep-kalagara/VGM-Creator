#pragma once

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "log.h"
#include "miniz.h"

struct InstrumentInfo {
    std::string name;
    std::string description;
    std::string path;
    std::unordered_map<std::string, float> settings;
};

struct Instrument {
    InstrumentInfo info;
};

static bool readInstrument(const std::string& zipPath,
                           InstrumentInfo* outInfo,
                           VGMLogger logger) {
    namespace fs = std::filesystem;

    if (!fs::exists(zipPath)) {
        logger.log(LOG_LEVEL_ERROR, "ZIP does not exist: %s", zipPath.c_str());
        return false;
    }

    uintmax_t size = fs::file_size(zipPath);
    if (size == 0) {
        logger.log(LOG_LEVEL_ERROR, "ZIP is empty: %s", zipPath.c_str());
        return false;
    }

    std::vector<unsigned char> buffer(size);

    // Load ZIP into memory
    std::ifstream file(zipPath, std::ios::binary);
    if (!file) {
        logger.log(LOG_LEVEL_ERROR, "Failed to open ZIP with ifstream: %s", zipPath.c_str());
        return false;
    }

    file.read(reinterpret_cast<char*>(buffer.data()), size);
    file.close();

    mz_zip_archive zip{};
    if (!mz_zip_reader_init_mem(&zip, buffer.data(), buffer.size(), 0)) {
        logger.log(LOG_LEVEL_ERROR, "Failed to read ZIP from memory: %s", zipPath.c_str());
        return false;
    }

    int fileCount = mz_zip_reader_get_num_files(&zip);

    // Find JSON file
    int jsonIndex = -1;
    mz_zip_archive_file_stat jsonStat{};

    for (int i = 0; i < fileCount; ++i) {
        mz_zip_archive_file_stat st{};
        if (!mz_zip_reader_file_stat(&zip, i, &st))
            continue;

        std::string name = st.m_filename;

        if (name.size() >= 5 && name.substr(name.size() - 5) == ".json") {
            jsonIndex = i;
            jsonStat = st;
            break;
        }
    }

    if (jsonIndex < 0) {
        logger.log(LOG_LEVEL_ERROR, "No JSON config found in ZIP: %s", zipPath.c_str());
        mz_zip_reader_end(&zip);
        return false;
    }

    logger.log(LOG_LEVEL_INFO, "Found JSON config: %s", jsonStat.m_filename);

    // Extract JSON to memory
    std::vector<char> jsonBuffer(jsonStat.m_uncomp_size);
    if (!mz_zip_reader_extract_to_mem(&zip, jsonIndex, jsonBuffer.data(), jsonBuffer.size(), 0)) {
        logger.log(LOG_LEVEL_ERROR, "Failed to extract JSON file.");
        mz_zip_reader_end(&zip);
        return false;
    }

    // Convert to string
    std::string jsonText(jsonBuffer.begin(), jsonBuffer.end());

    // Parse JSON
    nlohmann::json json;
    try {
        json = nlohmann::json::parse(jsonText);
    } catch (...) {
        logger.log(LOG_LEVEL_ERROR, "JSON parse error in: %s", zipPath.c_str());
        mz_zip_reader_end(&zip);
        return false;
    }

    if (!json.contains("Name") || !json.contains("Description") || !json.contains("Settings")) {
        logger.log(LOG_LEVEL_ERROR, "Missing required fields in JSON.");
        mz_zip_reader_end(&zip);
        return false;
    }

    if (outInfo) {
        outInfo->name = json.value("Name", "");
        outInfo->description = json.value("Description", "");
        outInfo->path = zipPath;

        for (auto& item : json["Settings"].items()) {
            outInfo->settings[item.key()] = item.value().get<float>();
        }
    }

    mz_zip_reader_end(&zip);
    return true;
}
