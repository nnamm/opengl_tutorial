#pragma once

// Drawing shapes
#include "Shape.h"

// Drawing shapes with index
class ShapeIndex : public Shape {
  protected:
    // The number of vertices used in the shape
    const GLsizei indexcount;

  public:
    // Constructor
    //   size       : Dimension of the vertex position
    //   vertexcount: Number of vertices
    //   vertex     : Array containing the vertex attributes
    //   indexcount : Number of elements at the vertex index
    //   index      : Array containing the indices of the vertices
    ShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount, const GLuint *index)
        : Shape(size, vertexcount, vertex, indexcount, index), indexcount(indexcount) {}

    // Execute drawing
    void execute() const override {
        // Drawing by line segment group
        glDrawElements(GL_LINES, indexcount, GL_UNSIGNED_INT, nullptr);
    }
};