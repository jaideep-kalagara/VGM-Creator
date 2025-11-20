// app.cpp
#include "app.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <nfd.hpp>
#include <vector>

#include "gui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void App::init() {
    m_logger = VGMLogger();

    if (!glfwInit()) {
        m_logger.log(LOG_LEVEL_ERROR, "Failed to initialize GLFW");
        return;
    }

    m_logger.log(LOG_LEVEL_INFO, "VGM Creator version %s", VGM_VERSION);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(800, 600, "VGM Creator", nullptr, nullptr);
    if (!m_window) {
        m_logger.log(LOG_LEVEL_ERROR, "Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        m_logger.log(LOG_LEVEL_ERROR, "Failed to initialize GLAD");
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return;
    }

    m_logger.log(LOG_LEVEL_INFO, "OpenGL version loaded: %s", glGetString(GL_VERSION));

    NFD::Init();

    // Setup Dear ImGui context
    m_gui = std::make_unique<Gui>(m_window, m_logger);
}

void App::run() {
    while (shouldRun()) {
        glClearColor(0.133f, 0.412f, 0.612f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_gui->update();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void App::terminate() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
    NFD::Quit();
    m_logger.log(LOG_LEVEL_INFO, "Application terminated");
}
