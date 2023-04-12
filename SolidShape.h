#pragma once

// Drawing shape
#include "Shape.h"

// Triangle drawing
class SolidShape : public Shape {
  public:
    // Constructor
    //   size       : Dimension of the vertex position
    //   vertexcount: Number of vertices
    //   vertex     : Array containing the vertex attributes
    SolidShape(GLint size, GLsizei vertexcount, const Object::Vertex *vertex) : Shape(size, vertexcount, vertex) {}

    // Execute drawing
    //    void execute() const override {
    void execute() const override {
        // Drawing by line segment group
        glDrawArrays(GL_TRIANGLES, 0, vertexcount);
    }
};