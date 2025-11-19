// app.h
#pragma once
#include <GLAD/gl.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include "log.h"

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

    std::unordered_map<std::string, bool> m_WindowStates = {
        {"Main Info", true}};

    void initializeImGui();
    void updateImGui();

    bool shouldRun() {
        return !glfwWindowShouldClose(m_window);
    }
};