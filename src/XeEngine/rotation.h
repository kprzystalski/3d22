//
// Created by Piotr Białas on 02/11/2020.
//

#pragma once

#include<cmath>

#include "glm/glm.hpp"

inline glm::mat3 rotation(float angle, const glm::vec3 &axis) {
    auto u = glm::normalize(axis);
    auto s = std::sin(angle);
    auto c = std::cos(angle);

    return glm::mat3(
            c + u.x * u.x * (1.0f - c),
            u.y * u.x * (1.0f - c) + u.z * s,
            u.z * u.x * (1.0f - c) - u.y * s,

            u.x *u.y*(1.0f-c)-u.z *s,
            c + u.y*u.y *(1.0f-c),
            u.z*u.y*(1.0f-c)+u.x*s,

            u.x*u.z*(1.0f -c)+ u.y*s,
            u.y*u.z*(1.0f-c)-u.x*s,
            c+u.z*u.z*(1.0f -c)
    );
}