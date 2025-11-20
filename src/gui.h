#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "editor.h"
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
    std::unordered_map<std::string, bool> m_WindowStates = {
        {"Main Info", true},
        {"Credits", false},
        {"Editor", false},
    };

    std::unique_ptr<Editor> m_editor;

    GLFWwindow* m_window = nullptr;
    VGMLogger m_logger;
};
