//
// Created by Piotr Białas on 03/11/2021.
//

#pragma once

#include <string>
#include <array>

#include "glad/gl.h"
#include "glm/glm.hpp"

#include "Node.h"
#include "lights.h"

namespace xe {

    const int MAX_POINT_LIGHT = 16;
    const int P_LIGHT_SIZE = 12 * sizeof(float);

    class Camera;


    class Scene {
    public:
        Scene();

        void set_root(Node *node) { root_ = node; }

        Camera *const camera() const { return camera_; }

        void set_camera(Camera *camera) { camera_ = camera; }

        void add_light(const PointLight &p_light) {
            p_lights_[n_lights_++] = p_light;
        }

        void load_transform(const GLfloat *M);
        void load_matrices(const glm::mat4& VM, const glm::mat3& N);

        void draw();

    private:
        GLuint u_transform_buffer_;
        GLuint u_matrices_buffer_;
        GLuint u_lights_buffer_;

        Node *root_;
        Camera *camera_;

        unsigned int n_lights_;
        std::array<PointLight, MAX_POINT_LIGHT> p_lights_;


    };

}



