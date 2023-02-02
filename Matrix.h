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

    // Set the unit matrix
    void loadIdentity() {
        std::fill(matrix, matrix + 16, 0.0f);
        matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
    }

    // Create the unit matrix
    static Matrix identity() {
        Matrix t;
        t.loadIdentity();
        return t;
    }

    // Create a transformation matrix that translates by (x, y, z)
    static Matrix translate(GLfloat x, GLfloat y, GLfloat z) {
        Matrix t;
        t.loadIdentity();
        t[12] = x;
        t[13] = y;
        t[14] = z;
        return t;
    }

    // Create a transformation matrix that scales (x, y, z) times
    static Matrix scale(GLfloat x, GLfloat y, GLfloat z) {
        Matrix t;
        t.loadIdentity();
        t[0] = x;
        t[5] = y;
        t[10] = z;
        return t;
    }
};