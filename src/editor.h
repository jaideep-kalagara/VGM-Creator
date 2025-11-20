#pragma once

#include <GLFW/glfw3.h>

#include <vector>

#include "instrument.h"
#include "log.h"

struct EditorInfo {
    VGMLogger* logger;
    std::vector<Instrument>* instruments;
    int* currentInstrumentIndex;
};

class Editor {
   public:
    Editor(GLFWwindow* window, VGMLogger logger) : m_window(window), m_logger(logger) {}
    ~Editor() = default;

    void getGui();

    EditorInfo getEditorInfo() {
        return EditorInfo{
            &m_logger,
            &m_musicalInstruments,
            &m_currentInstrumentIndex};
    }

   private:
    GLFWwindow* m_window = nullptr;
    VGMLogger m_logger;

    std::vector<Instrument> m_musicalInstruments;
    int m_currentInstrumentIndex = -1;

    void addMusicalInstrument();
};
