//
// Created by Piotr Białas on 25/10/2021.
//

#pragma once

#include <vector>
#include "glad/gl.h"


namespace xe {

    class Material;

    struct SubMesh {
        SubMesh(GLuint start, GLuint end, bool cull_face = false) : start(start), end(end), cull_face(cull_face) {}

        GLuint start;
        GLuint end;
        bool cull_face;

        GLuint count() const { return end - start; }
    };

    class Mesh {
    public:

        enum Attributes {
            COORDS = 0, TEX_COORDS = 1, NORMALS = 2, TANGENTS = 3
        };


        Mesh();

        void allocate_vertex_buffer(size_t size, GLenum hint);

        void allocate_index_buffer(size_t size, GLenum hint);

        void load_vertices(size_t offset, size_t size, const void *data);

        void load_indices(size_t offset, size_t size, const void *data);

        void vertex_attrib_pointer(GLuint index, GLuint size, GLenum type, GLsizei stride, GLsizeiptr offset);

        void add_submesh(GLuint start, GLuint end, Material *mtl = nullptr, bool cull_face = false) {
            submeshes_.push_back({start, end, cull_face});
            materials_.push_back(mtl);

        }

        void *map_vertex_buffer();

        void unmap_vertex_buffer();


        void *map_index_buffer();

        void unmap_index_buffer();


        void draw() const;

    private:

        GLuint vao_;
        GLuint v_buffer_;
        GLuint i_buffer_;

        std::vector<SubMesh> submeshes_;
        std::vector<Material *> materials_;

    };

}
