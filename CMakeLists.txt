cmake_minimum_required(VERSION 3.15)
project(VoxelSpaceEngine)

add_subdirectory(raylib)

add_compile_options(-O3)

add_executable(VoxelSpaceEngine main.c include/Structures.h lib/Structures.c lib/Renderer.c include/Renderer.h include/voxelspace.h)

target_link_libraries(VoxelSpaceEngine raylib)
target_include_directories(VoxelSpaceEngine PUBLIC include)
