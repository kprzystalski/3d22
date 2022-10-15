//
// Created by Piotr Białas on 26/11/2021.
//

#pragma once

#include <string>
#include <memory>
#include <vector>

#include "glm/glm.hpp"


namespace xe {

    class Scene;

    class Mesh;

    class Node {
    public:

        static Node *clone(const Node *node);

        Node(const std::string &name) : name_(name), parent_(nullptr), global_(1.0f), global_orientation_(1),
                                        local_(1.0f), local_orientation_(1),
                                        meshes_() {}

        Node() : name_(), parent_(nullptr), global_(1.0f), local_(1.0f), meshes_() {}

        Node(const Node &node);

        glm::mat4 local() const { return local_; }

        glm::mat4 global() const { return global_; }

        void set_parent(Node *parent) { parent_ = parent; }

        void set_local(const glm::mat4 &M, int orientation = 1) {
            local_ = M;
            local_orientation_ = orientation;
        }

        void add_node(Node *node) {
            node->set_parent(this);
            children_.push_back(node);
        }

        void draw(Scene *scene);

        void add_mesh(std::shared_ptr<xe::Mesh> pMesh);

    private:
        std::string name_;
        Node *parent_;
        std::vector<Node *> children_;
        glm::mat4 global_;
        int global_orientation_;
        glm::mat4 local_;
        int local_orientation_;
        std::vector<std::shared_ptr<xe::Mesh> > meshes_;
    };
}



