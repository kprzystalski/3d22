//
// Created by Piotr Białas on 03/11/2021.
//

#include "Scene.h"

#include "glm/gtc/type_ptr.hpp"

#include "Application/utils.h"
#include "Camera.h"

namespace xe {

    Scene::Scene() : n_lights_(0) {
        glGenBuffers(1, &u_transform_buffer_);
        glBindBuffer(GL_UNIFORM_BUFFER, u_transform_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_transform_buffer_);


        glGenBuffers(1, &u_lights_buffer_);
        glBindBuffer(GL_UNIFORM_BUFFER, u_lights_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, MAX_POINT_LIGHT * P_LIGHT_SIZE, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, u_lights_buffer_);

        glGenBuffers(1, &u_matrices_buffer_);
        glBindBuffer(GL_UNIFORM_BUFFER, u_matrices_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, 32 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_matrices_buffer_);

    }

    void Scene::load_transform(const GLfloat *M) {
        glBindBuffer(GL_UNIFORM_BUFFER, u_transform_buffer_);
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), M));
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
    }

    void Scene::draw() {
        // send lights;

        glBindBuffer(GL_UNIFORM_BUFFER, u_lights_buffer_);
        size_t offset = 0;
        for (int i = 0; i < n_lights_; i++) {
            auto pos = glm::vec4(p_lights_[i].position_in_world_space, 1.0f);
            p_lights_[i].position_in_view_space = glm::vec3(camera()->view() * pos);

            OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, P_LIGHT_SIZE, &p_lights_[i].position_in_view_space));
            offset += P_LIGHT_SIZE;
        }

        if (root_ != nullptr)
            root_->draw(this);
    }

    void Scene::load_matrices(const glm::mat4& VM, const glm::mat3&N ) {
        glBindBuffer(GL_UNIFORM_BUFFER, u_matrices_buffer_);
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), glm::value_ptr(VM)));
        for(int i=0;i<3;i++)
            OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(float)+i*4* sizeof(float), 3 * sizeof(float), glm::value_ptr(N[i])));
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
    }

}