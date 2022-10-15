//
// Created by Piotr Białas on 20/11/2021.
//

#include "PhongMaterial.h"

#include "Application/utils.h"
#include "XeEngine/utils.h"
#include "spdlog/spdlog.h"

namespace xe {

    GLuint PhongMaterial::shader_ = 0u;
    GLuint PhongMaterial::material_uniform_buffer_ = 0u;
    GLint  PhongMaterial::uniform_map_Kd_location_ = 0;

    void PhongMaterial::bind() {
        glUseProgram(program());
        int use_map_Kd = 0;
        if (map_Kd_ > 0) {
            OGL_CALL(glUniform1i(uniform_map_Kd_location_, map_Kd_unit_));
            OGL_CALL(glActiveTexture(GL_TEXTURE0 + map_Kd_unit_));
            OGL_CALL(glBindTexture(GL_TEXTURE_2D, map_Kd_));
            use_map_Kd = 1;
        }
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, material_uniform_buffer_));

        glBindBuffer(GL_UNIFORM_BUFFER, material_uniform_buffer_);
        glBufferSubData(GL_UNIFORM_BUFFER, 4* sizeof(float), sizeof(glm::vec4), &Kd_[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 15 * sizeof(float), sizeof(GLint), &use_map_Kd);
        OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0u));

    }

    void PhongMaterial::unbind() {
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
        glBindTexture(GL_TEXTURE_2D, 0u);
    }

    void PhongMaterial::init() {


        auto program = xe::utils::create_program(
                {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/phong_vs.glsl"},
                 {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/phong_fs.glsl"}});
        if (!program) {
            std::cerr << "Invalid program" << std::endl;
            exit(-1);
        }

        shader_ = program;

        glGenBuffers(1, &material_uniform_buffer_);

        glBindBuffer(GL_UNIFORM_BUFFER, material_uniform_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, 18* sizeof(float), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
#if __APPLE__
        uniform_block_binding(shader_, "Material",0);
#endif

#if __APPLE__
        uniform_block_binding(shader_, "Transformations",1);
#endif

#if __APPLE__
        uniform_block_binding(shader_, "Matrices",2);
#endif

#if __APPLE__
        uniform_block_binding(shader_, "Lights",3);
#endif


        uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
        if (uniform_map_Kd_location_ == -1) {
            spdlog::warn("Cannot get uniform {} location", "map_Kd");
        }

    }


}