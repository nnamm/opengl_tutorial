#version 150 core
#extension GL_ARB_explicit_attrib_location: enable
uniform mat4 modelview;
uniform mat4 projection;
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
//flat out vec4 vertex_color;
out vec4 vertex_color;
void main() {
    vertex_color = color;
    gl_Position = projection * modelview * position;
}
