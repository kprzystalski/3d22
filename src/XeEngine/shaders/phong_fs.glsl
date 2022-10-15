#version 460

#define MAX_POINT_LIGHTS 16

layout(location=0) out vec4 vFragColor;


#if __VERSION__ > 410
layout(std140, binding=0) uniform Material {
#else
    layout(std140) uniform Material {
#endif
    vec4  Ka; //0
    vec4  Kd; //4
    vec4  Ks; //8
    float Ns; //12
    float Ns_offset; //13
    bool use_map_Ka; //14
    bool use_map_Kd; //15
    bool use_map_Ks; //16
    bool use_map_Ns; //17
} material;



uniform vec3 ambient_light;


struct PointLight {
    vec3 position_in_view_space;
    vec3 color;
    vec3 atn;
};

#if __VERSION__ > 410
layout(std140, binding=3) uniform Light {
#else
layout(std140) uniform Lights {
#endif
    PointLight light[MAX_POINT_LIGHTS];
} p_light;


in vec2 vertex_texcoords_0;
in vec3 vertex_coords_in_viewspace;
in vec3 vertex_normal_in_viewspace;


uniform sampler2D map_Ka;
uniform sampler2D map_Kd;
uniform sampler2D map_Ks;

uniform sampler2D maps[3];

void main() {
vec4 Ka = material.Ka;
vec4 Kd = material.Kd;
vec4 Ks = material.Ks;
float Ns = material.Ns;

if(material.use_map_Ka)
    Ka *= texture(map_Ka, vertex_texcoords_0);
if (material.use_map_Kd)
    Kd *= texture(map_Kd, vertex_texcoords_0);
if(material.use_map_Ks)
    Ks.rgb *= texture(map_Ks, vertex_texcoords_0).rgb;
if(material.use_map_Ns)
    Ns *= texture(map_Ks, vertex_texcoords_0).a;

vFragColor = Kd;
}
