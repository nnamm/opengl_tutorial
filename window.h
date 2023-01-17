#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Window-related process
class Window {
    // Window handle
    GLFWwindow *const window;
    // Window size
    GLfloat size[2]{};
    // Scale factor of device coordinate system relative to world coordinate system
    GLfloat scale;

  public:
    // Constructor
    explicit Window(int width = 640, int height = 480, const char *title = "Hello!")
        : window(glfwCreateWindow(width, height, title, nullptr, nullptr)), scale(100.0f) {
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

        // Record this pointer of this instance
        glfwSetWindowUserPointer(window, this);

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

        // Get this pointer for this instance
        auto *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
        if (instance != nullptr) {
            // Save the size of the opened window
            instance->size[0] = static_cast<GLfloat>(width);
            instance->size[1] = static_cast<GLfloat>(height);
        }
    }

    // Retrieve the window size
    [[nodiscard]] const GLfloat *getSize() const { return size; }

    // Retrieve the scale factor
    [[nodiscard]] GLfloat getScale() const { return scale; }
};