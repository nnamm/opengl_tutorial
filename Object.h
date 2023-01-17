#pragma once
#include <GL/glew.h>

class Object {
    // Vertex array object name
    GLuint vao{};

    // Vertex buffer object name
    GLuint vbo{};

  public:
    // Vertex attribute
    struct Vertex {
        // Position
        GLfloat position[2];
    };

    // Constructor
    //   size       : Dimension of the vertex position
    //   vertexcount: Number of vertices
    //   vertex     : Array containing the vertex attributes
    Object(GLint size, GLsizei vertexcount, const Vertex *vertex) {
        // Vertex array object
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Vertex buffer object
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

        // Allow bound vertex buffer object to be reference from the in-variable
        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
    }

    // Destructor
    virtual ~Object() {
        // Delete vao
        glDeleteVertexArrays(1, &vao);

        // Delete vbo
        glDeleteBuffers(1, &vbo);
    }

  private:
    // Copy constructor prohibits copying
    Object(const Object &o);

    // Copy prohibition by assignment
    Object &operator=(const Object &o);

  public:
    // Merging of vertex array objects
    void bind() const {
        // Specifies vertex array objects to be drawn
        glBindVertexArray(vao);
    }
};