//
// Created by Piotr Białas on 13/12/2021.
//

#include "utils.h"

#include "spdlog/spdlog.h"

void uniform_block_binding(GLuint program, const std::string &name, GLuint binding) {
    auto u_index = glGetUniformBlockIndex(program, name.c_str());
    if (u_index == -1) {
        spdlog::warn("Cannot find  {} uniform block in program", name.c_str());
    } else {
        glUniformBlockBinding(program, u_index, binding);
    }
}

