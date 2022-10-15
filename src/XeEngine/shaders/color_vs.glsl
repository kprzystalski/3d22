#version 460

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec2 a_vertex_texcoords_0;
layout(location=2) in vec2 a_vertex_texcoords_1;
layout(location=3) in vec2 a_vertex_texcoords_2;
layout(location=4) in vec2 a_vertex_texcoords_3;

#if __VERSION__ > 410
layout(std140, binding=1) uniform Transformations {
#else
    layout(std140) uniform Transformations {
    #endif
    mat4 PVM;
};

out vec2 vertex_texcoords_0;

void main() {
    vertex_texcoords_0 = a_vertex_texcoords_0;
    gl_Position =  PVM * a_vertex_position;
}
