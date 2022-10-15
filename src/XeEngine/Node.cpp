//
// Created by Piotr Białas on 26/11/2021.
//

#include "Node.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "spdlog/spdlog.h"

#include "Camera.h"
#include "Scene.h"

#include "Mesh.h"


namespace xe {

    Node *Node::clone(const Node *node) {
        auto ptr = new Node(*node);

        for (auto &&ch: node->children_) {
            ptr->add_node(clone(ch));

        }
        return ptr;
    }

    Node::Node(const Node &node) :
            name_(node.name_),
            parent_(nullptr),
            children_(),
            global_(1.0f),
            global_orientation_(1),
            local_(node.local_),
            local_orientation_(node.local_orientation_),
            meshes_(node.meshes_) {
        name_ = node.name_;
        parent_ = nullptr;
    }

    void Node::draw(Scene *scene) {

        if (parent_ != nullptr) {
            global_ = parent_->global_ * local_;
            global_orientation_ = parent_->global_orientation_ * local_orientation_;
            spdlog::debug("Drawing node {} {} {}", parent_->name_, name_, global_orientation_);
        } else {
            global_ = local_;
            global_orientation_ = local_orientation_;
            spdlog::debug("Drawing node {}", name_, global_orientation_);
        }
        if (global_orientation_ > 0) {
            glFrontFace(GL_CCW);
        } else {
            glFrontFace(GL_CW);
        }

        auto VM = scene->camera()->view() * global_;
        auto PVM = scene->camera()->projection() *VM;
        auto R = glm::mat3(VM);
        auto N = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1]));
        scene->load_transform(glm::value_ptr(PVM));
        scene->load_matrices(VM,N);

        for (auto &&m: meshes_) {
            m->draw();
        }


        for (auto &&ch: children_) {
            ch->draw(scene);
        }
    }

    void Node::add_mesh(std::shared_ptr<xe::Mesh> pMesh) {
        meshes_.push_back(pMesh);
    }
}