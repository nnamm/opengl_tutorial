#pragma once
#include <array>

// Transformation Matrix
#include "Matrix.h"

// Vector
using Vector = std::array<GLfloat, 4>;

// Matrix and Vector Multiplication
//   m: Matrix of type matrix
//   n: Vector of type vector
Vector operator*(const Matrix &m, const Vector &v) {
    Vector t;
    for (int i = 0; i < 4; ++i) {
        t[i] = m[i] * v[0] + m[i + 4] * v[1] + m[i + 8] * v[2] + m[i + 12] * v[3];
    }
    return t;
}