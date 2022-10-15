//
// Created by Piotr Białas on 20/11/2021.
//

#pragma once

#include "Material.h"

#include <string>

namespace xe {
    class PhongMaterial : public Material {
    public:

        static void init();

        static GLuint program() { return shader_; }

        PhongMaterial(const glm::vec4 color, GLuint texture, GLuint texture_unit) : Kd_(color), map_Kd_(texture),
                                                                                    map_Kd_unit_(texture_unit) {}

        PhongMaterial(const glm::vec4 color, GLuint texture) : PhongMaterial(color, texture, 0) {}

        PhongMaterial(const glm::vec4 color) : PhongMaterial(color, 0) {}

        void set_texture(GLuint tex) { map_Kd_ = tex; }

        void bind() override;

        void unbind() override;


    private:

        static GLuint shader_;
        static GLuint material_uniform_buffer_;
        static GLint uniform_map_Kd_location_;

        glm::vec4 Kd_;
        GLuint map_Kd_;
        GLboolean use_map_Kd_;
        GLuint map_Kd_unit_;
    };


    GLuint create_texture(const std::string &name);

}




