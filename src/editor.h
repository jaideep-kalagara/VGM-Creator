// editor.h
#pragma once

#include <GLAD/gl.h>
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <vector>

#include "log.h"

class Editor {
   public:
    Editor(GLFWwindow* window, VGMLogger logger) : m_window(window), m_logger(logger) {};
    ~Editor() {};

    void getGui();

   private:
    GLFWwindow* m_window = nullptr;
    VGMLogger m_logger;

    std::vector<std::string> m_musicalInstruments;
    int m_currentInstrumentIndex = -1;

    void addMusicalInstrument();
};