#version 460

layout(location=0) out vec4 vFragColor;

#if __VERSION__ > 410
layout(std140, binding=0) uniform Color {
#else
    layout(std140) uniform Color {
    #endif
    vec4  Kd;
    bool use_map_Kd;
};

in vec2 vertex_texcoords_0;

uniform sampler2D map_Kd;

void main() {
    if (use_map_Kd)
    vFragColor = Kd*texture(map_Kd, vertex_texcoords_0);
    else
    vFragColor = Kd;
    //vFragColor = vec4(1.0, 0.0, 0.0, 1.0);
}