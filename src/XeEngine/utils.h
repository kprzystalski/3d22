//
// Created by Piotr Białas on 13/12/2021.
//

#pragma once

#include <string>

#include "glad/gl.h"

void uniform_block_binding(GLuint program, const std::string& name, GLuint binding);
