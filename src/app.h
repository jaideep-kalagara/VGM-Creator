// app.h
#pragma once
#include <GLAD/gl.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "gui.h"
#include "log.h"

#define VGM_VERSION "0.0.1"

class App {
   public:
    App() {};
    ~App() {};

    void init();
    void run();
    void terminate();

   private:
    VGMLogger m_logger;
    GLFWwindow* m_window;

    std::unique_ptr<Gui> m_gui;

    bool shouldRun() {
        return !glfwWindowShouldClose(m_window);
    }
};