#version 150 core
#extension GL_ARB_explicit_attrib_location: enable
in vec3 Idiff;
in vec3 Ispec;
layout (location = 0) out vec4 fragment;
void main() {
    fragment = vec4(Idiff + Ispec, 1.0);
}