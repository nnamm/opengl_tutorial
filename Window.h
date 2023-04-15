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
    // Location on the normalized device coordinate system of the figure
    GLfloat location[2];
    // Key status
    int keyStatus;

  public:
    // Constructor
    explicit Window(int width = 640, int height = 480, const char *title = "Hello!")
        : window(glfwCreateWindow(width, height, title, nullptr, nullptr)), scale(100.0f), location{0.0f, 0.0f},
          keyStatus(GLFW_RELEASE) {

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

        // Register callback process when the mouse wheel is operated
        glfwSetScrollCallback(window, wheel);

        // Register callback process at the time of keyboard operation
        glfwSetKeyCallback(window, keyboard);

        // Record this pointer of this instance
        glfwSetWindowUserPointer(window, this);

        // Initial settings for opened window
        resize(window, width, height);
    }

    // Destructor
    virtual ~Window() { glfwDestroyWindow(window); }

    // Continuation check of the drawing loop
    explicit operator bool() {
        // Extract events
        glfwPollEvents();

        // Check keyboard status
        if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)
            location[0] -= 2.0f / size[0];
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
            location[0] += 2.0f / size[0];
        if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
            location[1] -= 2.0f / size[1];
        else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
            location[1] += 2.0f / size[1];

        // Check left mouse button
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE) {
            // Get mouse cursor position if the left mouse button is pressed
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            // Find the mouse cursor's position on the normalized device coordinate system
            location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
            location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
        }

        // Return true if the window does not need to be closed
        return !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE);
    }

    // Double buffering
    void swapBuffers() const {
        // Replace the color buffer
        glfwSwapBuffers(window);
    }

    // Handling of window resizing
    static void resize(GLFWwindow *window, int width, int height) {
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

    // Handling of mouse wheel operating
    static void wheel(GLFWwindow *window, double x, double y) {
        // Get this pointer for this instance
        auto *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
        if (instance != nullptr) {
            // Update the scaling factor(x5) of the device coordinate system
            // relative to the world coordinate system
            instance->scale += static_cast<GLfloat>(y) * 5;
        }
    }

    // Handling of keyboard operating
    static void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
        // Get this pointer for this instance
        auto *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
        if (instance != nullptr) {
            // Save key status
            instance->keyStatus = action;
        }
    }

    // Retrieve the window size
    [[nodiscard]] const GLfloat *getSize() const { return size; }

    // Retrieve the scale factor
    [[nodiscard]] GLfloat getScale() const { return scale; }

    // Retrieve the position
    [[nodiscard]] const GLfloat *getLocation() const { return location; }
};