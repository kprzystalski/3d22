//
// Created by Piotr Białas on 13/12/2021.
//

#pragma once

#include "glm/glm.hpp"

struct PointLight {
    PointLight() = default;

    PointLight(const glm::vec3 &pos, const glm::vec3 &color, const glm::vec3 &atn) : position_in_world_space(pos),
                                                                                     color(color), atn(atn) {}

    alignas(16) glm::vec3 position_in_world_space;
    alignas(16) glm::vec3 position_in_view_space;
    alignas(16) glm::vec3 color;
    alignas(16) glm::vec3 atn;
};