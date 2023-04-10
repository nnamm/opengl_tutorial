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
    explicit Matrix(const GLfloat *a) { std::copy(a, a + 16, matrix); }

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

    // Create view transformation matrix
    static Matrix lookat(GLfloat ex, GLfloat ey, GLfloat ez, // Position of viewpoint
                         GLfloat gx, GLfloat gy, GLfloat gz, // Location of target point
                         GLfloat ux, GLfloat uy, GLfloat uz  // Upward vector
    ) {
        // Transformation matrix of translate
        const Matrix tv(translate(-ex, -ey, -ez));

        // Axis-t = e - g
        const GLfloat tx(ex - gx);
        const GLfloat ty(ey - gy);
        const GLfloat tz(ez - gz);

        // Axis-r = u * t axis
        const GLfloat rx(uy * tz - uz * ty);
        const GLfloat ry(uz * tx - ux * tz);
        const GLfloat rz(ux * ty - uy * tx);

        // Axis-s = Axis-t * Axis-r
        const GLfloat sx(ty * rz - tz * ry);
        const GLfloat sy(tz * rx - tx * rz);
        const GLfloat sz(tx * ry - ty * rx);

        // Check axis length
        const GLfloat s2(sx * sx + sy * sy + sz * sz);
        if (s2 == 0.0f)
            return tv;

        // Transformation matrix of rotation
        Matrix rv;
        rv.loadIdentity();

        // r axis normalized and stored in array variable
        const GLfloat r(sqrt(rx * rx + ry * ry + rz * rz));
        rv[0] = rx / r;
        rv[4] = ry / r;
        rv[8] = rz / r;

        // s-axis normalized and stored in array variables
        const GLfloat s(sqrt(s2));
        rv[1] = sx / s;
        rv[5] = sy / s;
        rv[9] = sz / s;

        // t-axis normalized and stored in array variables
        const GLfloat t(sqrt(tx * tx + ty * ty + tz * tz));
        rv[2] = tx / t;
        rv[6] = ty / t;
        rv[10] = tz / t;

        // Multiply the transformation matrix of the translation of the viewpoint
        // by the transformation matrix of the rotation of the gaze
        return rv * tv;
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

    // Create the orthogonal projection transformation matrix
    static Matrix orthogonal(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) {
        Matrix t;
        const GLfloat dx(right - left);
        const GLfloat dy(top - bottom);
        const GLfloat dz(zFar - zNear);

        if (dx != 0.0f && dy != 0.0f && dz != 0.0f) {
            t.loadIdentity();
            t[0] = 2.0f / dx;
            t[5] = 2.0f / dy;
            t[10] = -2.0f / dz;
            t[12] = -(right + left) / dx;
            t[13] = -(top + bottom) / dy;
            t[14] = -(zFar - zNear) / dz;
        }

        return t;
    }

    // Create the perspective projection transformation matrix
    static Matrix frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) {
        Matrix t;
        const GLfloat dx(right - left);
        const GLfloat dy(top - bottom);
        const GLfloat dz(zFar - zNear);

        if (dx != 0.0f && dy != 0.0f && dz != 0.0f) {
            t.loadIdentity();
            t[0] = 2.0f * zNear / dx;
            t[5] = 2.0f * zNear / dy;
            t[8] = (right + left) / dx;
            t[9] = (top + bottom) / dy;
            t[10] = -(zFar + zNear) / dz;
            t[11] = -1.0f;
            t[14] = -2.0f * zFar * zNear / dz;
            t[15] = 0.0f;
        }

        return t;
    }

    // Create the perspective projection transformation matrix by specifying the angle of view
    static Matrix perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar) {
        Matrix t;
        const GLfloat dz(zFar - zNear);

        if (dz != 0.0f) {
            t.loadIdentity();
            t[5] = 1.0f / tan(fovy * 0.5f);
            t[0] = t[5] / aspect;
            t[10] = -(zFar + zNear) / dz;
            t[11] = -1.0f;
            t[14] = -2.0f * zFar * zNear / dz;
            t[15] = 0.0f;
        }
        return t;
    }
};
