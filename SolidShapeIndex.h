#pragma once

// Drawing shapes with index
#include "ShapeIndex.h"

// Triangle drawing with index
class SolidShapeIndex : public ShapeIndex {
  public:
    // Constructor
    //   size       : Dimension of the vertex position
    //   vertexcount: Number of vertices
    //   vertex     : Array containing the vertex attributes
    //   indexcount : Number of elements at the vertex index
    //   index      : Array containing the indices of the vertices
    SolidShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex, GLsizei indexcount,
                    const GLuint *index)
        : ShapeIndex(size, vertexcount, vertex, indexcount, index) {}

    // Execute drawing
    void execute() const override {
        // Drawing by line segment group
        glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, nullptr);
    }
};