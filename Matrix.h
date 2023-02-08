#pragma once
#include <GL/glew.h>
#include <algorithm>
#include <cmath>

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

    // Create a transformation matrix with a rotation around (x, y, z)
    static Matrix rotate(GLfloat a, GLfloat x, GLfloat y, GLfloat z) {
        Matrix t;
        const GLfloat d(sqrt(x * x + y * y + z * z));

        if (d > 0.0f) {
            const GLfloat l(x / d), m(y / d), n(z / d);
            const GLfloat l2(l * l), m2(m * m), n2(n * n);
            const GLfloat lm(l * m), mn(m * n), nl(n * l);
            const GLfloat c(cos(a)), c1(1.0f - c), s(sin(a));

            t.loadIdentity();
            t[0] = (1.0f - l2) * c + l2;
            t[1] = lm * c1 + n * s;
            t[2] = nl * c1 - m * s;
            t[4] = lm * c1 - n * s;
            t[5] = (1.0f - m2) * c + m2;
            t[6] = mn * c1 + l * s;
            t[8] = nl * c1 + m * s;
            t[9] = mn * c1 - l * s;
            t[10] = (1.0f - n2) * c + n2;
        }

        return t;
    }

    // Multiplication
    Matrix operator*(const Matrix &m) const {
        Matrix t;
        for (int i = 0; i < 16; i++) {
            const int j(i & 3), k(i & ~3);
            t[i] = matrix[0 + j] * m[k + 0] + matrix[4 + j] * m[k + 1] + matrix[8 + j] * m[k + 2] +
                   matrix[12 + j] * m[k + 3];
        }
        return t;
    }
};
