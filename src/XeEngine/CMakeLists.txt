cmake_minimum_required(VERSION 3.15)
project(XeEngine CXX)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_library(xe-engine
        Camera.h
        Material.h
        ColorMaterial.cpp ColorMaterial.h
        Scene.cpp Scene.h
        Mesh.cpp Mesh.h
        mesh_loader.cpp mesh_loader.h
        Node.cpp Node.h
        PhongMaterial.cpp PhongMaterial.h
        stb_image.cpp lights.h
        utils.h utils.cpp)

target_link_libraries(xe-engine PUBLIC objreader PRIVATE spdlog::spdlog)