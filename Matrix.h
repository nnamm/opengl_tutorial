#pragma once
#include <GL/glew.h>
#include <algorithm>

// Transformation matrix
class Matrix {
    GLfloat matrix[16]{};

  public:
    Matrix() = default;

    // Constructor initialized with the contents of an array
    //  a: 16-element array of GLfloat type
    Matrix(const GLfloat *a) { std::copy(a, a + 16, matrix); }

    // Referring to an element of a matrix as the right-hand side value
    const GLfloat &operator[](std::size_t i) const { return matrix[i]; }

    // Referring to an element of a matrix as the left-hand side value
    GLfloat &operator[](std::size_t i) { return matrix[i]; }

    // Returns an array of transformation matrices
    [[nodiscard]] const GLfloat *data() const { return matrix; }
};