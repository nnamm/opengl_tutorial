#version 150 core
#extension GL_ARB_explicit_attrib_location: enable
flat in vec4 vertex_color;
layout (location = 0) out vec4 fragment;
void main() {
    //    fragment = vec4(1.0, 0.0, 0.0, 1.0);
    fragment = vertex_color;
}