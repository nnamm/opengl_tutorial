#version 150 core
#extension GL_ARB_explicit_attrib_location: enable
uniform mat4 modelview;
uniform mat4 projection;
const vec4 Lpos =  vec4(0.0, 0.0, 5.0, 1.0);
const vec3 Ldiff = vec3(1.0);
const vec3 Kdiff = vec3(0.6, 0.6, 0.2);
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
out vec3 Idiff;
void main() {
    vec4 P = modelview * position;
    vec3 L = normalize((Lpos * P.w - P *Lpos.w).xyz);
    Idiff = max(dot(normal, L), 0.0) * Kdiff * Ldiff;
    gl_Position = projection * P;
}
