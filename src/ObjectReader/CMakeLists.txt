cmake_minimum_required(VERSION 3.15)
project(OBJReader CXX)

add_library(objreader
        obj_reader.cpp  obj_reader.h
        sMesh.h
        )

target_link_libraries(objreader PRIVATE spdlog::spdlog)