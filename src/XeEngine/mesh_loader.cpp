//
// Created by Piotr Białas on 04/11/2021.
//



#include "mesh_loader.h"

#include <memory>


#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ObjectReader/obj_reader.h"
#include "XeEngine/ColorMaterial.h"
#include "XeEngine/PhongMaterial.h"
#include "XeEngine/Mesh.h"


namespace {
    xe::ColorMaterial *make_color_material(const xe::mtl_material_t &mat, std::string mtl_dir);
    xe::PhongMaterial *make_phong_material(const xe::mtl_material_t &mat, std::string mtl_dir);
}

namespace xe {


    std::shared_ptr<Mesh> load_mesh_from_obj(std::string path, std::string mtl_dir) {

        auto smesh = xe::load_smesh_from_obj(path, mtl_dir);
        if (smesh.vertex_coords.empty())
            return nullptr;


        auto mesh = new Mesh;
        auto n_vertices = smesh.vertex_coords.size();
        auto n_indices = 3 * smesh.faces.size();


        uint n_floats_per_vertex = 3;
        for (auto &&t: smesh.has_texcoords) {
            if (t)
                n_floats_per_vertex += 2;
        }
        if (smesh.has_normals)
            n_floats_per_vertex += 3;
        if (smesh.has_normals)
            n_floats_per_vertex += 4;


        size_t stride = n_floats_per_vertex * sizeof(GLfloat);


        mesh->allocate_index_buffer(n_indices * sizeof(uint16_t), GL_STATIC_DRAW);
        mesh->load_indices(0, n_indices * sizeof(uint16_t), smesh.faces.data());

        mesh->allocate_vertex_buffer(n_vertices * n_floats_per_vertex * sizeof(float), GL_STATIC_DRAW);
        mesh->vertex_attrib_pointer(0, 3, GL_FLOAT, n_floats_per_vertex * sizeof(GLfloat), 0);


        auto v_ptr = reinterpret_cast<uint8_t *>(mesh->map_vertex_buffer());

        size_t offset = 0;

        {

            auto v_offset = offset;
            for (auto i = 0; i < smesh.vertex_coords.size(); i++, v_offset += stride) {
                spdlog::debug("vertex[{}] {} ", i, glm::to_string(smesh.vertex_coords[i]));
                std::memcpy(v_ptr + v_offset, glm::value_ptr(smesh.vertex_coords[i]), sizeof(glm::vec3));
            }
        }
        offset += 3 * sizeof(GLfloat);

        for (int it = 0; it < xe::sMesh::MAX_TEXCOORDS; it++) {
            if (smesh.has_texcoords[it]) {
                mesh->vertex_attrib_pointer(1 + it, 2, GL_FLOAT, stride, offset);

                auto v_offset = offset;
                for (auto i = 0; i < smesh.vertex_texcoords[it].size(); i++, v_offset += stride) {
                    std::memcpy(v_ptr + v_offset, glm::value_ptr(smesh.vertex_texcoords[0][i]), sizeof(glm::vec2));
                }
                offset += 2 * sizeof(GLfloat);
            }
        }
        if (smesh.has_normals) {
            mesh->vertex_attrib_pointer(xe::sMesh::MAX_TEXCOORDS + 1, 3, GL_FLOAT, stride, offset);
            offset += 3 * sizeof(GLfloat);
        }

        if (smesh.has_tangents) {
            mesh->vertex_attrib_pointer(xe::sMesh::MAX_TEXCOORDS + 2, 4, GL_FLOAT, stride, offset);
            offset += 4 * sizeof(GLfloat);
        }

        mesh->unmap_vertex_buffer();


        for (int i = 0; i < smesh.submeshes.size(); i++) {
            auto sm = smesh.submeshes[i];
            spdlog::debug("Adding submesh {:4d} {:4d} {:4d}", i, sm.start, sm.end);
            Material* material = new xe::ColorMaterial(glm::vec4{1.0, 1.0, 1.0, 1.0});
            if (sm.mat_idx >= 0) {
                auto mat = smesh.materials[sm.mat_idx];
                switch (mat.illum) {
                    case 0:
                        material = make_color_material(mat, mtl_dir);
                        break;
                    case 1:
                        material = make_phong_material(mat, mtl_dir);
                        break;
                }

                mesh->add_submesh(sm.start, sm.end, material, false);
            }

        }
        return std::shared_ptr<Mesh>(mesh);


    }
}

    namespace {

        xe::ColorMaterial *make_color_material(const xe::mtl_material_t &mat, std::string mtl_dir) {

            glm::vec4 color;
            for (int i = 0; i < 3; i++)
                color[i] = mat.diffuse[i];
            color[3] = 1.0;
            spdlog::debug("Adding ColorMaterial {}", glm::to_string(color));
            auto material = new xe::ColorMaterial(color);
            if (!mat.diffuse_texname.empty()) {
                auto texture = xe::create_texture(mtl_dir + "/" + mat.diffuse_texname);
                spdlog::debug("Adding Texture {} {:1d}", mat.diffuse_texname, texture);
                if (texture > 0) {
                    material->set_texture(texture);
                }
            }

            return material;
        }

        xe::PhongMaterial *make_phong_material(const xe::mtl_material_t &mat, std::string mtl_dir) {

            glm::vec4 color;
            for (int i = 0; i < 3; i++)
                color[i] = mat.diffuse[i];
            color[3] = 1.0;
            spdlog::debug("Adding ColorMaterial {}", glm::to_string(color));
            auto material = new xe::PhongMaterial(color);
            if (!mat.diffuse_texname.empty()) {
                auto texture = xe::create_texture(mtl_dir + "/" + mat.diffuse_texname);
                spdlog::debug("Adding Texture {} {:1d}", mat.diffuse_texname, texture);
                if (texture > 0) {
                    material->set_texture(texture);
                }
            }

            return material;
        }

    }