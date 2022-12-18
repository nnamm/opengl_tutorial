#pragma once
#include "Object.h"
#include <memory>

class Shape {
    // Graphic data
    std::shared_ptr<const Object> object;

  protected:
    // Number of vertices used for drawing
    const GLsizei vertexcount;

  public:
    // Constructor
    //   size       : Dimension of the vertex position
    //   vertexcount: Number of vertices
    //   vertex     : Array containing the vertex attributes
    Shape(GLint size, GLsizei vertexcount, const Object::Vertex *vertex)
        : object(new Object(size, vertexcount, vertex)), vertexcount(vertexcount) {}

    void draw() const {
        // Merge vertex array object
        object->bind();
        // Execute drawing
        execute();
    }

    virtual void execute() const { glDrawArrays(GL_LINE_LOOP, 0, vertexcount); }
};