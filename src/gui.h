#pragma once

#include <GLAD/gl.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "log.h"

class Gui {
   public:
    Gui(GLFWwindow* window, VGMLogger logger);
    ~Gui();

    void update();

   private:
    GLFWwindow* m_window = nullptr;
    VGMLogger m_logger;

    std::vector<std::string> m_musicalInstruments;
    int m_currentInstrumentIndex = -1;

    std::unordered_map<std::string, bool> m_WindowStates = {
        {"Main Info", true},
        {"Credits", false},
        {"Editor", false},
    };

    void addMusicalInstrument();
};
