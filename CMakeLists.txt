cmake_minimum_required(VERSION 3.5)
project(game_of_life)

set(CMAKE_CXX_STANDARD 17)

include(vendors/raylib.cmake)
include(vendors/raylib_cpp.cmake)
include(vendors/imgui.cmake)
include(vendors/rlimgui.cmake)

add_subdirectory(core)