#version 460

layout(location=0) in vec4 a_vertex_position;


layout(location=1) in vec2 a_vertex_texcoords_0;
layout(location=2) in vec2 a_vertex_texcoords_1;
layout(location=3) in vec2 a_vertex_texcoords_2;
layout(location=4) in vec2 a_vertex_texcoords_3;
layout(location=5) in vec3 a_vertex_normal;


#if __VERSION__ > 410
layout(std140, binding=1) uniform Transformations {
#else
    layout(std140) uniform Transformations {
#endif
    mat4 PVM;
};
#if __VERSION__ > 410
layout(std140, binding=2) uniform Matrices {
#else
    layout(std140) uniform Matrices {
#endif
    mat4 VM;
    mat3 N;
};


out vec2 vertex_texcoords_0;
out vec3 vertex_coords_in_viewspace;
out vec3 vertex_normal_in_viewspace;


void main() {

    vertex_texcoords_0 = a_vertex_texcoords_0;
    vec4 vertex_coords_in_viewspace4 = VM*a_vertex_position;
    vertex_coords_in_viewspace = vertex_coords_in_viewspace4.xyz/vertex_coords_in_viewspace4.w;
    vertex_normal_in_viewspace = normalize(N * a_vertex_normal);
    gl_Position =  PVM*a_vertex_position;
}
