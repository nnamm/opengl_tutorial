#version 150 core
in vec4 vertex_color;
out vec4 fragment;
void main() {
    //    fragment = vec4(1.0, 0.0, 0.0, 1.0);
    fragment = vertex_color;
}