#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Window-related process
class Window {
    // Window handle
    GLFWwindow *const window;

  public:
    // Constructor
    explicit Window(int width = 640, int height = 480, const char *title = "Hello!")
        : window(glfwCreateWindow(width, height, title, nullptr, nullptr)) {
        if (window == nullptr) {
            // can not create window
            std::cerr << "Can't create GLFW window." << std::endl;
            exit(1);
        }

        // Make the current window the target of processing
        glfwMakeContextCurrent(window);

        // Init GLEW
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            // Fail init
            std::cerr << "Can't initialize GLEW" << std::endl;
            exit(1);
        }

        // Wait for the vertical sync timing
        glfwSwapInterval(1);

        // Register callback process when the window is resized
        glfwSetWindowSizeCallback(window, resize);
        // Initial settings for opened window
        resize(window, width, height);
    }

    // Destructor
    virtual ~Window() { glfwDestroyWindow(window); }

    // Continuation check of the drawing loop
    explicit operator bool() {
        // Extract events
        glfwWaitEvents();
        // Return true if the window does not need to be closed
        return !glfwWindowShouldClose(window);
    }

    // Double buffering
    void swapBuffers() const {
        // Replace the color buffer
        glfwSwapBuffers(window);
    }

    // Handling of window resizing
    static void resize(GLFWwindow *const window, int width, int height) {
        // Find the size of the frame buffer
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        // set the entire frame buffer to be viewport
        glViewport(0, 0, fbWidth, fbHeight);
    }
};