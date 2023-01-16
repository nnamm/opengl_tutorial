#include "Shape.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

// Display the compiled result of shader object
//   shader: Shader object name
//   str   : String indicating where the compilation error occurred
GLboolean printShaderInfoLog(GLuint shader, const char *str) {
    // Get compilation result
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
        std::cerr << "Compile Error in " << str << std::endl;

    // Get log length when shader is compiled
    GLsizei bufSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

    if (bufSize > 1) {
        // Get log contents
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }

    return static_cast<GLboolean>(status);
}

// Display the link result of program object
//   program: program object name
GLboolean printProgramInfoLog(GLuint program) {
    // Get link result
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        std::cerr << "Link Error." << std::endl;

    // Get log length when linking shader
    GLsizei bufSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

    if (bufSize > 1) {
        // Get log contents
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }

    return static_cast<GLboolean>(status);
}

// Create program object
//   vsrc: Vertex shader source program string
//   fsrc: Fragment shader source program string
GLuint createProgram(const char *vsrc, const char *fsrc) {
    // Create empty object
    const GLuint program(glCreateProgram());

    if (vsrc != nullptr) {
        // Create shader object for vertex shader
        const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
        glShaderSource(vobj, 1, &vsrc, nullptr);
        glCompileShader(vobj);

        // Embed shader object of vertex shader into program object
        if (printShaderInfoLog(vobj, "vertex shader"))
            glAttachShader(program, vobj);
        glDeleteShader(vobj);
    }

    if (fsrc != nullptr) {
        // Create shader object for fragment shader
        const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
        glShaderSource(fobj, 1, &fsrc, nullptr);
        glCompileShader(fobj);

        // Embed shader object of fragment shader into program object
        if (printShaderInfoLog(fobj, "fragment shader"))
            glAttachShader(program, fobj);
        glDeleteShader(fobj);
    }

    // Link program object
    glBindAttribLocation(program, 0, "position");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);

    // Return created program object
    if (printProgramInfoLog(program))
        return program;

    // Return 0, if program object cannot be created
    glDeleteProgram(program);
    return 0;
}

// Returns the memory from which the shader source file was loaded
//   name  : Shader source file name
//   buffer: Text of the loaded source file
bool readShaderSource(const char *name, std::vector<GLchar> &buffer) {
    if (name == nullptr)
        return false;

    std::ifstream file(name, std::ios::binary);
    if (file.fail()) {
        std::cerr << "Error: Can't open source file: " << name << std::endl;
        return false;
    }

    // Move to the end of the file and get the current position(= file size)
    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());

    // Allocate file size memory
    buffer.resize(length + 1);

    // Read the file from the beginning
    file.seekg(0L, std::ios::beg);
    file.read(buffer.data(), length);
    buffer[length] = '\0';

    if (file.fail()) {
        std::cerr << "Error: Could not read source file: " << name << std::endl;
        file.close();
        return false;
    }

    // Read success
    file.close();
    return true;
}

// Read shader source file and create program object
//   vert: Vertex shader source file name
//   frag: Source file name of the fragment shader
bool loadProgram(const char *vert, const char *frag) {
    // Load shader source file
    std::vector<GLchar> vsrc;
    const bool vstat(readShaderSource(vert, vsrc));
    std::vector<GLchar> fsrc;
    const bool fstat(readShaderSource(frag, fsrc));

    // Create program object
    //    return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
    //    return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : false;
    return vstat && fstat && createProgram(vsrc.data(), fsrc.data());
}

//  Rectangle vertex position
constexpr Object::Vertex rectangleVertex[] = {
    {-0.5f, -0.5f},
    {1.5f, -0.5f},
    {1.5f, 1.5f},
    {-0.5f, 1.5f},
};

int main() {
    // Initialize GLFW
    if (glfwInit() == GL_FALSE) {
        std::cerr << "Can't initialize GLFW" << std::endl;
    }

    // Register processing at the end of program
    atexit(glfwTerminate);

    // Select OpenGL version 3.2 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create Window
    GLFWwindow *const window(glfwCreateWindow(640, 480, "Hello", nullptr, nullptr));
    if (window == nullptr) {
        std::cerr << "Can't create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Make the created window an OpenGL target
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Can't initialize GLEW" << std::endl;
        return 1;
    }

    // Wait for V-sync timing
    glfwSwapInterval(1);

    // Set background color
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // Set viewport
    glViewport(100, 50, 300, 300);

    // Create program object
    const GLuint program(loadProgram("point.vert", "point.frag"));

    // Create graphic data
    std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));

    // Repeat while the window is open
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT);

        // Start using shader program
        glUseProgram(program);

        // rendering process
        shape->draw();

        // Replace the color buffer
        glfwSwapBuffers(window);

        // Extract events
        glfwWaitEvents();
    }
}