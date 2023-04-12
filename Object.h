#pragma once
#include <GL/glew.h>

class Object {
    // Vertex array object name
    GLuint vao{};

    // Vertex buffer object name
    GLuint vbo{};

    // Index vertex buffer object
    GLuint ibo{};

  public:
    // Vertex attribute
    struct Vertex {
        // Position
        GLfloat position[3];

        // Color
        GLfloat color[3];
    };

    // Constructor
    //   size       : Dimension of the vertex position
    //   vertexcount: Number of vertices
    //   vertex     : Array containing the vertex attributes
    //   indexcount : Number of elements at the vertex index
    //   index      : Array containing the indices of the vertices
    Object(GLint size, GLsizei vertexcount, const Vertex *vertex, GLsizei indexcount = 0,
           const GLuint *index = nullptr) {
        // Vertex array object
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Vertex buffer object
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

        // Allow bound vertex buffer object to be reference from the in-variable
        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex *>(0)->position);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex *>(0)->color);
        glEnableVertexAttribArray(1);

        // Index vertex buffer object
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
    }

    // Destructor
    virtual ~Object() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
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