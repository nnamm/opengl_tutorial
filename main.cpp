#include "Matrix.h"
#include "Shape.h"
#include "ShapeIndex.h"
#include "SolidShape.h"
#include "SolidShapeIndex.h"
#include "Window.h"
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
    glBindAttribLocation(program, 1, "color");
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
    return vstat && fstat && createProgram(vsrc.data(), fsrc.data());
}

// Vertex attributes of a hexahedron with a different color for each face
constexpr Object::Vertex solidCubeVertex[] = {
    // Left
    {{-1.0f, -1.0f, -1.0f}, {0.1f, 0.8f, 0.1f}},
    {{-1.0f, -1.0f, 1.0f}, {0.1f, 0.8f, 0.1f}},
    {{-1.0f, 1.0f, 1.0f}, {0.1f, 0.8f, 0.1f}},
    {{-1.0f, -1.0f, -1.0f}, {0.1f, 0.8f, 0.1f}},
    {{-1.0f, 1.0f, 1.0f}, {0.1f, 0.8f, 0.1f}},
    {{-1.0f, 1.0f, -1.0f}, {0.1f, 0.8f, 0.1f}},
    // Back
    {{1.0f, -1.0f, -1.0f}, {0.8f, 0.1f, 0.8f}},
    {{-1.0f, -1.0f, -1.0f}, {0.8f, 0.1f, 0.8f}},
    {{-1.0f, 1.0f, -1.0f}, {0.8f, 0.1f, 0.8f}},
    {{1.0f, -1.0f, -1.0f}, {0.8f, 0.1f, 0.8f}},
    {{-1.0f, 1.0f, -1.0f}, {0.8f, 0.1f, 0.8f}},
    {{1.0f, 1.0f, -1.0f}, {0.8f, 0.1f, 0.8f}},
    // Under
    {{-1.0f, -1.0f, -1.0f}, {0.1f, 0.8f, 0.8f}},
    {{1.0f, -1.0f, -1.0f}, {0.1f, 0.8f, 0.8f}},
    {{1.0f, -1.0f, 1.0f}, {0.1f, 0.8f, 0.8f}},
    {{-1.0f, -1.0f, -1.0f}, {0.1f, 0.8f, 0.8f}},
    {{1.0f, -1.0f, 1.0f}, {0.1f, 0.8f, 0.8f}},
    {{-1.0f, -1.0f, 1.0f}, {0.1f, 0.8f, 0.8f}},
    // Right
    {{1.0f, -1.0f, 1.0f}, {0.1f, 0.1f, 0.8f}},
    {{1.0f, -1.0f, -1.0f}, {0.1f, 0.1f, 0.8f}},
    {{1.0f, 1.0f, -1.0f}, {0.1f, 0.1f, 0.8f}},
    {{1.0f, -1.0f, 1.0f}, {0.1f, 0.1f, 0.8f}},
    {{1.0f, 1.0f, -1.0f}, {0.1f, 0.1f, 0.8f}},
    {{1.0f, 1.0f, 1.0f}, {0.1f, 0.1f, 0.8f}},
    // Up
    {{-1.0f, 1.0f, -1.0f}, {0.8f, 0.1f, 0.1f}},
    {{-1.0f, 1.0f, 1.0f}, {0.8f, 0.1f, 0.1f}},
    {{1.0f, 1.0f, 1.0f}, {0.8f, 0.1f, 0.1f}},
    {{-1.0f, 1.0f, -1.0f}, {0.8f, 0.1f, 0.1f}},
    {{1.0f, 1.0f, 1.0f}, {0.8f, 0.1f, 0.1f}},
    {{1.0f, 1.0f, -1.0f}, {0.8f, 0.1f, 0.1f}},
    // Front
    {{-1.0f, -1.0f, 1.0f}, {0.8f, 0.8f, 0.1f}},
    {{1.0f, -1.0f, 1.0f}, {0.8f, 0.8f, 0.1f}},
    {{1.0f, 1.0f, 1.0f}, {0.8f, 0.8f, 0.1f}},
    {{-1.0f, -1.0f, 1.0f}, {0.8f, 0.8f, 0.1f}},
    {{1.0f, 1.0f, 1.0f}, {0.8f, 0.8f, 0.1f}},
    {{-1.0f, 1.0f, 1.0f}, {0.8f, 0.8f, 0.1f}}};

// Index of triangularity vertices that fill the faces with a hexahedron
constexpr GLuint solidCubeIndex[] = {
    0,  1,  2,  3,  4,  5,  // Left
    6,  7,  8,  9,  10, 11, // Back
    12, 13, 14, 15, 16, 17, // Under
    18, 19, 20, 21, 22, 23, // Right
    24, 25, 26, 27, 28, 29, // Up
    30, 31, 32, 33, 34, 35  // Front
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

    Window window;

    // Set background color
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // Enable back culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Create program object
    const GLuint program(loadProgram("point.vert", "point.frag"));

    // Get uniform variable location
    const GLint modelviewLoc(glGetUniformLocation(program, "modelview"));
    const GLint projectionLoc(glGetUniformLocation(program, "projection"));

    // Create graphic data
    std::unique_ptr<const Shape> shape(new SolidShapeIndex(3, 36, solidCubeVertex, 36, solidCubeIndex));

    // Set timer 0
    glfwSetTime(0.0);

    // Repeat while the window is open
    while (window) {
        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT);

        // Start using shader program
        glUseProgram(program);

        // Calculate the perspective projection transformation matrix
        const GLfloat *const size(window.getSize());
        const GLfloat fovy(window.getScale() * 0.01f);
        const GLfloat aspect(size[0] / size[1]);
        const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));

        // Calculate the model transformation matrix
        const GLfloat *const location(window.getLocation());
        const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 1.0f, 0.0f));
        const Matrix model(Matrix::translate(location[0], location[1], 0.0f) * r);

        // Calculate the view transformation matrix
        const Matrix view(Matrix::lookat(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

        // Calculate the model view transformation matrix
        const Matrix modelview(view * model);

        // Set a value to uniform variable
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());

        // Drawing shape
        shape->draw();

        // Replace the color buffer
        window.swapBuffers();
    }
}
