//
// Created by Piotr Białas on 12/11/2021.
//

#pragma once

#include <cmath>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "XeEngine/rotation.h"

namespace {
    inline float logistic(float y) {
        return 1.0f / (1.0f + std::exp(-y));
    }

    inline float inverse_logistics(float x) {
        return std::log(x / (1.0f - x));
    }
}


namespace xe {
    class Camera {
    public:

        glm::vec3 x() const { return x_; }

        glm::vec3 y() const { return y_; }

        glm::vec3 z() const { return z_; }

        glm::vec3 position() const { return position_; }

        glm::vec3 center() const { return center_; }

        void look_at(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
            z_ = glm::normalize(eye - center);
            x_ = glm::normalize(glm::cross(up, z_));
            y_ = glm::normalize(glm::cross(z_, x_));

            position_ = eye;
            center_ = center;
        }

        void perspective(float fov, float aspect, float near, float far) {
            fov_ = fov;
            aspect_ = aspect;
            near_ = near;
            far_ = far;
        }

        void set_aspect(float aspect) {
            aspect_ = aspect;
        }

        glm::mat4 view() const {
            glm::mat4 V(1.0f);
            for (int i = 0; i < 3; ++i) {
                V[i][0] = x_[i];
                V[i][1] = y_[i];
                V[i][2] = z_[i];
            }

            auto t = -glm::vec3{
                    glm::dot(x_, position_),
                    glm::dot(y_, position_),
                    glm::dot(z_, position_),
            };
            V[3] = glm::vec4(t, 1.0f);

            return V;

        }

        glm::mat4 projection() const { return glm::perspective(fov_, aspect_, near_, far_); }

        void zoom(float y_offset) {
            auto y = inverse_logistics(fov_ / glm::pi<float>());
            y += y_offset;
            fov_ = logistic(y) * glm::pi<float>();
        }


        void rotate_around_point(float angle, const glm::vec3 &axis, const glm::vec3 &c) {
            auto R = rotation(angle, axis);
            x_ = R * x_;
            y_ = R * y_;
            z_ = R * z_;

            auto t = position_ - c;
            t = R * t;
            position_ = c + t;

        }

        void rotate_around_center(float angle, const glm::vec3 &axis) {
            rotate_around_point(angle, axis, center_);
        }


    private:
        float fov_;
        float aspect_;
        float near_;
        float far_;


        glm::vec3 position_;
        glm::vec3 center_;
        glm::vec3 x_;
        glm::vec3 y_;
        glm::vec3 z_;
    };

}

