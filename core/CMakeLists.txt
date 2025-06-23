set(SOURCE_FILES CACHE INTERNAL "")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    raylib
    raylib_cpp
    imgui
    rlimgui
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

file(GLOB_RECURSE SOURCE_FILES src/ *.cpp)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${SOURCE_FILES}
)

install(
    TARGETS ${PROJECT_NAME}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)